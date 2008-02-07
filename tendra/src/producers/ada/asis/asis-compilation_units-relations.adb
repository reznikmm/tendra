------------------------------------------------------------------------------
--                           G E L A   A S I S                              --
--       ASIS implementation for Gela project, a portable Ada compiler      --
--                     http://www.ten15.org/wiki/Ada                        --
--                     - - - - - - - - - - - - - - -                        --
--            Read copyright and license at the end of this file            --
------------------------------------------------------------------------------
--  $TenDRA$
--  Purpose:
--  Procedural wrapper over Object-Oriented ASIS implementation

------------------------------------------------------------------------------
--                      Implementation restriction                          --
--             not implemented Inconsistent list generation                 --
------------------------------------------------------------------------------

with Asis.Errors;             use Asis.Errors;
with Asis.Exceptions;
with Asis.Implementation;
with Asis.Ada_Environments;

with Ada.Finalization;
with Ada.Unchecked_Deallocation;
with System;

package body Asis.Compilation_Units.Relations is

   package Utils is

      --  Compilation_Unit_List_Access  --
      type Compilation_Unit_List_Access is
        access all Compilation_Unit_List;

      procedure Deallocate is
        new Ada.Unchecked_Deallocation
          (Compilation_Unit_List, Compilation_Unit_List_Access);

      function In_List
        (List : in Compilation_Unit_List_Access;
         Last : in ASIS_Integer;
         Unit : in Compilation_Unit)
         return Boolean;

      procedure Remove_From_List
        (List : in out Compilation_Unit_List;
         From : in     List_Index;
         Unit : in     Compilation_Unit);

      function Append
        (List : in Compilation_Unit_List_Access;
         Unit : in Compilation_Unit)
         return Compilation_Unit_List_Access;

      function Append
        (List  : in Compilation_Unit_List_Access;
         Units : in Compilation_Unit_List)
         return Compilation_Unit_List_Access;

      --  Tree_Node  --
      type Tree_Node is
        new Ada.Finalization.Limited_Controlled with private;

      type Tree_Node_Access is access all Tree_Node;

      function Is_Empty
        (This : in Tree_Node)
         return Boolean;

      function Append_Parent
        (This : in Tree_Node_Access;
         Unit : in Compilation_Unit)
         return Tree_Node_Access;

      procedure Add_Thread
        (This      : in     Tree_Node_Access;
         To_Node   : in     Tree_Node_Access;
         From_Tree : in out Tree_Node_Access);

      function Add_Child
        (This      : in Tree_Node_Access;
         Node      : in Tree_Node_Access;
         Spec_Unit : in Compilation_Unit;
         Body_Unit : in Compilation_Unit)
         return Tree_Node_Access;

      procedure Glue_Nodes
        (This    : in Tree_Node_Access;
         Node    : in Tree_Node_Access;
         To_Node : in Tree_Node_Access);

      procedure Clear
        (This : in out Tree_Node);

      function Find
        (This : in Tree_Node;
         Unit : in Compilation_Unit)
         return Tree_Node_Access;

      type Orders is (Ascending, Descending);

      procedure Check
        (This  : in Tree_Node_Access;
         Order : in Orders);

      function Generate_Relationship
        (This       : in Tree_Node_Access;
         Limit_List : in Utils.Compilation_Unit_List_Access;
         List_Last  : in ASIS_Integer;
         Order      : in Orders)
         return Relationship;

      Use_Error : exception;

   private

      type Tree_Node_Array is array (Positive range <>) of Tree_Node_Access;
      type Tree_Node_Array_Access is access all Tree_Node_Array;

      type Unit_Node is record
         Unit : Compilation_Unit;
         Node : Tree_Node_Access;
      end record;

      type Unit_Node_Array is array (Positive range <>) of Unit_Node;
      type Unit_Node_Array_Access is access all Unit_Node_Array;

      type Tree_Node is
        new Ada.Finalization.Limited_Controlled with record
         Self : Tree_Node_Access := Tree_Node'Unchecked_Access;

         --  ссылка на предыдущий елемент
         Prev : Tree_Node_Access := null;

         --  модуль_компиляции
         Unit      : Compilation_Unit := Nil_Compilation_Unit;
         Unit_Body : Compilation_Unit := Nil_Compilation_Unit;

         Added      : Boolean := False;
         Consistent : Boolean := True;

         --  последующие елементы 0-х
         Next : Tree_Node_Array_Access := null;

         --  список окончания "ветвей",
         --  для быстрого изымания оных
         --  а не перебором всех ветвей
         --  заполняет только корневой елемент
         Last_Nodes : Tree_Node_Array_Access := null;

         --  сортированный список всех
         --  елементов для быстрого
         --  определения наличия елемента
         --  в списке
         --  заполняет только корневой елемент
         Units : Unit_Node_Array_Access := null;

         --  список циклических зависимостей
         Circular       : Compilation_Unit_List_Access := null;
         Circular_Added : Boolean := False;

         --  список пропавших юнитов
         Missing       : Compilation_Unit_List_Access := null;
         Missing_Added : Boolean := False;

         --  список несоглассованных юнитов
         Inconsistent       : Compilation_Unit_List_Access := null;
         Inconsistent_Added : Boolean := False;
      end record;

      procedure Finalize
        (This : in out Tree_Node);

      function Set_Inconsistent
        (This  : in Tree_Node_Access;
         List  : in Compilation_Unit_List_Access;
         Order : in Orders)
         return Compilation_Unit_List_Access;

      procedure Deallocate is
        new Ada.Unchecked_Deallocation
          (Unit_Node_Array, Unit_Node_Array_Access);

      type Positive_Access is access all Positive;

      function Add_Node
        (List : in Tree_Node_Array_Access;
         Node : in Tree_Node_Access)
         return Tree_Node_Array_Access;

      function Add_Node_Ordered
        (List : in Unit_Node_Array_Access;
         Node : in Tree_Node_Access)
         return Unit_Node_Array_Access;

      function Find
        (List  : in Unit_Node_Array_Access;
         Unit  : in Compilation_Unit;
         From  : in Positive;
         To    : in Positive;
         Index : in Positive_Access)
         return Boolean;

      function Compare
        (Left  : in Compilation_Unit;
         Right : in Compilation_Unit)
         return Integer;

      function Is_Inconsistent
        (Unit : in Compilation_Unit)
         return Boolean;

      function Is_Source_Changed
        (Unit : in Compilation_Unit)
         return Boolean;

   end Utils;

   procedure Deallocate is
     new Ada.Unchecked_Deallocation
       (Utils.Tree_Node, Utils.Tree_Node_Access);

   function Get_Ancestors
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access;

   function Get_Descendants
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access;

   function Get_Supporters
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access;

   -------------------------
   --  Elaboration_Order  -- *
   -------------------------

   function Elaboration_Order
     (Compilation_Units : in Asis.Compilation_Unit_List;
      The_Context       : in Asis.Context)
      return Relationship
   is
   begin
      Asis.Implementation.Set_Status
        (Not_Implemented_Error, "Elaboration_Order not implemented");
      raise Asis.Exceptions.ASIS_Failed;

      return Nil_Relationship;
   end Elaboration_Order;

   ---------------------------------
   --  Semantic_Dependence_Order  -- *
   ---------------------------------

   function Semantic_Dependence_Order
     (Compilation_Units : in Asis.Compilation_Unit_List;
      Dependent_Units   : in Asis.Compilation_Unit_List;
      The_Context       : in Asis.Context;
      Relation          : in Asis.Relation_Kinds)
      return Relationship
   is
      Current_Unit_Kind : Asis.Unit_Kinds;

      -- Check_Compilation_Unit --
      procedure Check_Compilation_Unit
         (Unit    : in Compilation_Unit;
          Message : in Wide_String)
      is
      begin
         Current_Unit_Kind := Unit_Kind (Unit);

         if Current_Unit_Kind = Not_A_Unit
            or else Current_Unit_Kind = A_Nonexistent_Declaration
            or else Current_Unit_Kind = A_Nonexistent_Body
            or else Current_Unit_Kind = A_Configuration_Compilation
         then
            Asis.Implementation.Set_Status
              (Data_Error, "Semantic_Dependence_Order "
               & Message & " invalid");

            raise Asis.Exceptions.ASIS_Inappropriate_Compilation_Unit;
         end if;

         if not Asis.Ada_Environments.Is_Equal
           (Enclosing_Context (Unit), The_Context)
         then
            Asis.Implementation.Set_Status
              (Data_Error, "Semantic_Dependence_Order "
               & Message & " invalid context");

            raise Asis.Exceptions.ASIS_Inappropriate_Compilation_Unit;
         end if;
      end Check_Compilation_Unit;

      Compilation_Units_Last : ASIS_Integer := 0;

      Normalized_Compilation_Units :
         Utils.Compilation_Unit_List_Access := null;

      Dependent_Units_Last : ASIS_Integer := 0;

      Normalized_Dependent_Units :
         Utils.Compilation_Unit_List_Access := null;

      -- Normalize --
      procedure Normalize
         (List   : in     Asis.Compilation_Unit_List;
          Result : in     Utils.Compilation_Unit_List_Access;
          Last   :    out ASIS_Integer)
      is
         Unit : Compilation_Unit;
      begin
         Last := 0;

         for Index in List'Range loop
            Unit := List (Index);

            if Assigned (Unit)
               and then Unit_Kind (Unit) /= An_Unknown_Unit
            then
               if not Utils.In_List (Result, Last, Unit) then
                  Last := Last + 1;
                  Result (Last) := List (Index);
               end if;
            end if;
         end loop;
      end Normalize;

      Tree : Utils.Tree_Node_Access := null;

      procedure Clear is
      begin
         Deallocate (Tree);
         Utils.Deallocate (Normalized_Compilation_Units);
         Utils.Deallocate (Normalized_Dependent_Units);
      end Clear;

   begin
      if Compilation_Units = Nil_Compilation_Unit_List then
         return Nil_Relationship;
      end if;

      for Index in Compilation_Units'Range loop
         Check_Compilation_Unit
           (Compilation_Units (Index), "Compilation_Unit");
      end loop;

      Normalized_Compilation_Units := new
        Asis.Compilation_Unit_List (1 .. Compilation_Units'Length);

      Normalized_Compilation_Units.all := (others => Nil_Compilation_Unit);

      Normalize (Compilation_Units,
                 Normalized_Compilation_Units,
                 Compilation_Units_Last);

      --  Dependent_Units are ignored unless the Relation
      --  is Descendants or Dependents
      if Relation = Descendants
         or else Relation = Dependents
      then
         for Index in Dependent_Units'Range loop
            Check_Compilation_Unit (Dependent_Units (Index), "Dependent_Unit");
         end loop;

         Normalized_Dependent_Units := new
           Asis.Compilation_Unit_List (1 .. Dependent_Units'Length);

         Normalized_Dependent_Units.all := (others => Nil_Compilation_Unit);

         Normalize (Dependent_Units,
                    Normalized_Dependent_Units,
                    Dependent_Units_Last);
      end if;

      case Relation is
         when Ancestors =>
            Tree := Get_Ancestors
              (Normalized_Compilation_Units (1 .. Compilation_Units_Last),
               The_Context);

            Utils.Check (Tree, Utils.Ascending);
            declare
               Relation : Relationship := Utils.Generate_Relationship
                  (Tree, null, 0, Utils.Ascending);
            begin
               Clear;
               return Relation;
            end;

         when Descendants =>
            Tree := Get_Descendants
              (Normalized_Compilation_Units (1 .. Compilation_Units_Last), The_Context);

            Utils.Check (Tree, Utils.Ascending);
            declare
               Relation : Relationship := Utils.Generate_Relationship
                  (Tree, Normalized_Dependent_Units, Dependent_Units_Last, Utils.Descending);
            begin
               Clear;
               return Relation;
            end;

         when Supporters =>
            Tree := Get_Supporters
              (Normalized_Compilation_Units (1 .. Compilation_Units_Last),
               The_Context);

            Asis.Implementation.Set_Status
              (Not_Implemented_Error,
               "Semantic_Dependence_Order not implemented");

            raise Asis.Exceptions.ASIS_Failed;

         when Dependents =>
            Asis.Implementation.Set_Status
              (Not_Implemented_Error,
               "Semantic_Dependence_Order not implemented");

            raise Asis.Exceptions.ASIS_Failed;

         when Family =>
            Asis.Implementation.Set_Status
              (Not_Implemented_Error,
               "Semantic_Dependence_Order not implemented");
            raise Asis.Exceptions.ASIS_Failed;

         when Needed_Units =>
            Asis.Implementation.Set_Status
              (Not_Implemented_Error,
               "Semantic_Dependence_Order not implemented");
            raise Asis.Exceptions.ASIS_Failed;
      end case;

      Clear;
      return Nil_Relationship;

   exception
      when others =>
         Clear;
         raise;
   end Semantic_Dependence_Order;

   -------------------
   -- Get_Ancestors --
   -------------------

   function Get_Ancestors
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access
   is
      use Utils;

      Unit  : Compilation_Unit;
      Kinds : Unit_Kinds;

      Result   : Tree_Node_Access := null;
      Tmp_Tree : Tree_Node_Access := null;
      Node     : Tree_Node_Access := null;

      -- Append_Node --
      function Append_Node
        (Unit : in Compilation_Unit)
         return Boolean
      is
      begin
         if Result /= null then
            Node := Find (Result.all, Unit);
         else
            Node := null;
         end if;

         if Node /= null then
            Add_Thread (Result, Node, Tmp_Tree);
            return True;
         else
            Tmp_Tree := Append_Parent (Tmp_Tree, Unit);
            return False;
         end if;
      end Append_Node;

      -- Retrive_Declarations --
      procedure Retrive_Declarations is
      begin
         while Unit_Kind (Unit) in A_Procedure .. A_Generic_Package_Renaming loop
            if Append_Node (Unit) then
               return;
            end if;

            Unit := Corresponding_Parent_Declaration (Unit);
         end loop;

         if not Is_Nil (Unit) then
            if Append_Node (Unit) then
               return;
            end if;

            --  add Standart as root
            if Append_Node
              (Library_Unit_Declaration ("Standard", The_Context))
            then
               return;
            end if;
         end if;
      end Retrive_Declarations;

      --  Retrive_Subunit  --
--      procedure Retrive_Subunit is
--      begin
--         if Append_Node (Unit) then
--            return;
--         end if;

--         loop
--            Unit := Corresponding_Subunit_Parent_Body (Unit);

--            if Append_Node (Unit) then
--               return;
--            end if;

--            exit when Unit_Kind (Unit) not in A_Subunit;
--         end loop;

--         if Unit_Kind (Unit) /= A_Nonexistent_Body then
--            Unit := Corresponding_Parent_Declaration (Unit, The_Context);
--            Retrive_Declarations;
--         else
            --  add Standart as root
--            if Append_Node
--              (Library_Unit_Declaration ("Standard", The_Context))
--            then
--               return;
--            end if;
--         end if;
--      end Retrive_Subunit;

   begin
      for Index in List'Range loop
         if Tmp_Tree /= null then
            Asis.Implementation.Set_Status
              (Asis.Errors.Internal_Error,
               "Bug (or inapropriate use) detected in "
                & "Asis.Compilation_Units.Relations.Semantic_Dependence_Order(Ancestors)");

            raise Asis.Exceptions.ASIS_Failed;
         end if;

         Tmp_Tree := new Tree_Node;

         Unit  := List (Index);
         Kinds := Unit_Kind (Unit);

         if Kinds in A_Subunit then
--            Retrive_Subunit;
            null;

         elsif Kinds in A_Library_Unit_Body then
            Unit := Corresponding_Parent_Declaration (Unit, The_Context);
            Retrive_Declarations;

         else
            Retrive_Declarations;
         end if;

         if Result = null then
            Result   := Tmp_Tree;
            Tmp_Tree := null;
         end if;
      end loop;

      Deallocate (Tmp_Tree);
      return Result;
   exception
      when others =>
         Deallocate (Tmp_Tree);
         Deallocate (Result);
         raise;
   end Get_Ancestors;

   ---------------------
   -- Get_Descendants --
   ---------------------

   function Get_Descendants
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access
   is
      use Utils;

      Result      : Tree_Node_Access := new Tree_Node;
      Unit        : Compilation_Unit;
      Second_Unit : Compilation_Unit;
      Kinds       : Unit_Kinds;

      -- Retrive --
      procedure Retrive
        (Target : in Compilation_Unit;
         Node   : in Utils.Tree_Node_Access)
      is
         Children_List : Asis.Compilation_Unit_List :=
           Corresponding_Children (Target, The_Context);
         Exist_Node : Utils.Tree_Node_Access := null;

         -- Process --
         function Process
            (Index : in List_Index)
            return Boolean
         is
         begin
            if Is_Nil (Unit) then
               return False;
            end if;

            Exist_Node := Find (Result.all, Unit);

            if Exist_Node /= null then
               Glue_Nodes (Result, Node, Exist_Node);
               return False;
            end if;

            Kinds := Unit_Kind (Unit);

            if Kinds in
              A_Procedure_Instance .. A_Generic_Package_Renaming
            then
               Exist_Node := Add_Child (Result, Node, Unit, null);

            elsif Kinds in A_Procedure .. A_Generic_Package then
               Second_Unit := Corresponding_Body (Unit, The_Context);

               if not Is_Identical (Second_Unit, Nil_Compilation_Unit)
                  and then not Is_Identical (Second_Unit, Unit)
               then
                  Exist_Node := Add_Child (Result, Node, Unit, Second_Unit);
                  Remove_From_List (Children_List, Index + 1, Second_Unit);
               else
                  Exist_Node := Add_Child (Result, Node, Unit, null);
               end if;

            elsif Kinds in A_Library_Unit_Body then
               Second_Unit := Corresponding_Declaration (Unit, The_Context);

               if not Is_Identical (Second_Unit, Nil_Compilation_Unit)
                  and then not Is_Identical (Second_Unit, Unit)
               then
                  Exist_Node := Add_Child (Result, Node, Second_Unit, Unit);
                  Remove_From_List (Children_List, Index + 1, Second_Unit);
                  Unit := Second_Unit;
               else
                  Exist_Node := Add_Child (Result, Node, Unit, null);
               end if;

            else
               Exist_Node := Add_Child (Result, Node, Unit, null);
            end if;

            return True;
         end Process;

      begin
         for Index in Children_List'Range loop
            Unit := Children_List (Index);

            if Process (Index) then
               Kinds := Unit_Kind (Unit);

               if Kinds = A_Package
                  or else Kinds = A_Generic_Package
                  or else Kinds = A_Package_Instance
               then
                  Retrive (Unit, Exist_Node);
               end if;
            end if;
         end loop;
      end Retrive;

      Declarations_List :
         Utils.Compilation_Unit_List_Access := null;
      Declarations_Last : ASIS_Integer := 0;

   begin
      Declarations_List := new Asis.Compilation_Unit_List (1 .. List'Length);

      for Index in List'Range loop
         Unit  := List (Index);
         Kinds := Unit_Kind (Unit);

         --  eliminate A_Subunit
         if Kinds not in A_Subunit then
            if Kinds in A_Library_Unit_Body then
               --  get declaration
               Unit  := Corresponding_Declaration (Unit);
               Kinds := Unit_Kind (Unit);
            end if;

            if Kinds = A_Package
              or else Kinds = A_Generic_Package
              or else Kinds = A_Package_Instance
            then
               if not In_List
                 (Declarations_List, Declarations_Last, Unit)
               then
                  Declarations_Last := Declarations_Last + 1;
                  Declarations_List (Declarations_Last) := Unit;
               end if;
            end if;
         end if;
      end loop;

      for Index in 1 .. Declarations_Last loop
         Unit := Declarations_List (Index);

         if Find (Result.all, Unit) = null then
            Second_Unit := Corresponding_Body (Unit, The_Context);

            if not Is_Identical (Second_Unit, Nil_Compilation_Unit)
               and then not Is_Identical (Second_Unit, Unit)
            then
               Retrive (Unit, Add_Child (Result, null, Unit, Second_Unit));
            else
               Retrive (Unit, Add_Child (Result, null, Unit, null));
            end if;
         end if;
      end loop;

      Deallocate (Declarations_List);
      return Result;

   exception
      when others =>
         Deallocate (Declarations_List);
         Deallocate (Result);
         raise;
   end Get_Descendants;

   --------------------
   -- Get_Supporters --
   --------------------

   function Get_Supporters
     (List        : in Asis.Compilation_Unit_List;
      The_Context : in Asis.Context)
      return Utils.Tree_Node_Access
   is
      use Utils;

--      Unit  : Compilation_Unit;
--      Kinds : Unit_Kinds;

      Result : Tree_Node_Access := new Tree_Node;
--      Node   : Tree_Node_Access := null;

      -- Append_Standart --
--      procedure Append_Standart
--        (Node : in Tree_Node_Access)
--      is
--         Std : Compilation_Unit := Library_Unit_Declaration
--           ("Standard", The_Context);

--         Exist_Node : Tree_Node_Access;
--      begin
--         Exist_Node := Find (Result.all, Std);

--         if Exist_Node = null then
--            Exist_Node := Add_Child (Result, Node, Unit);
--         else
--            if Node /= null then
--               Glue_Nodes (Result, Node, Exist_Node);
--            end if;
--         end if;
--      end Append_Standart;

      -- Reorder --
--      procedure Reorder
--        (Unit : in Compilation_Unit;
--         Node : in Tree_Node_Access)
--      is
--      begin
--         Kinds := Unit_Kind (Unit);

--         if Is_Nill (Unit)
--           or else Kinds in A_Nonexistent_Declaration .. An_Unknown_Unit
--         then
--            Append_Standart (Node);

--         elsif Kinds in A_Subunit then
--            Retrive_Subunit (Unit, Node);

--         elsif Kinds = A_Package_Body then
--            Retrive_Body (Unit, Node);

--         elsif Kinds in A_Subprogram_Body then
--            Retrive_Subprogram_Body (Get_Package_Body (Unit), Node);

--         else
--            Retrive_Declarations (Unit, Node);
--         end if;
--      end Reorder;

      --  Retrive_Subunit  --
--      procedure Retrive_Subunit
--        (Unit : in Compilation_Unit;
--         Node : in Tree_Node_Access)
--      is
--         Parent     : Compilation_Unit;
--         Exist_Node : Tree_Node_Access := Node;
--      begin
--         Parent := Corresponding_Subunit_Parent_Body (Unit);

--         while Unit_Kind (Parent) in A_Subunit loop
--            Exist_Node := Find (Result.all, Parent);

--            if Exist_Node = null then
--               Exist_Node := Add_Child (Result, Node, Unit);
--            else
--               if Node /= null then
--                  Glue_Nodes (Result, Node, Exist_Node);
--                  return;
--               end if;
--            end if;

--            Parent := Corresponding_Subunit_Parent_Body (Unit);
--         end loop;

--         Reorder (Parent, Exist_Node);
--      end Retrive_Subunit;

   begin
--      for Index in List'Range loop
--         Unit := List (Index);
--         Reorder (List (Index), null);
--      end loop;

      return Result;
   exception
      when others =>
         Deallocate (Result);
         raise;
   end Get_Supporters;

   ------------
   --  Utils --
   ------------

   package body Utils is

      procedure Deallocate is
        new Ada.Unchecked_Deallocation
          (Tree_Node_Array, Tree_Node_Array_Access);

      --------------
      -- Is_Empty --
      --------------

      function Is_Empty
        (This : in Tree_Node)
         return Boolean
      is
      begin
         return Asis.Compilation_Units.Is_Nil (This.Unit);
      end Is_Empty;

      -------------------
      -- Append_Parent --
      -------------------

      function Append_Parent
        (This : in Tree_Node_Access;
         Unit : in Compilation_Unit)
         return Tree_Node_Access
      is
         Node : Tree_Node_Access;
      begin
         if Is_Empty (This.all) then
            This.Unit := Unit;

            This.Last_Nodes := Add_Node (This.Last_Nodes, This.Self);
            This.Units      := Add_Node_Ordered (This.Units, This.Self);
            return This;
         end if;

         if This.Prev /= null 
            or else Find (This.all, Unit) /= null
         then
            raise Use_Error;
         end if;

         Node := new Tree_Node;

         Node.Unit := Unit;
         This.Prev := Node.Self;
         Node.Next := Add_Node (Node.Next, This.Self);

         Node.Last_Nodes := This.Last_Nodes;
         This.Last_Nodes := null;

         Node.Units := This.Units;
         This.Units := null;
         Node.Units := Add_Node_Ordered (Node.Units, Node.Self);

         return Node;
      end Append_Parent;

      ----------------
      -- Add_Thread --
      ----------------

      procedure Add_Thread
        (This      : in     Tree_Node_Access;
         To_Node   : in     Tree_Node_Access;
         From_Tree : in out Tree_Node_Access)
      is
      begin
         if Is_Empty (From_Tree.all)
           or else Is_Empty (This.all)
         then
            --  empty tree(s)
            raise Use_Error;
         end if;

         if From_Tree.Prev /= null
           or else This.Prev /= null
         then
            --  not root
            raise Use_Error;
         end if;

         for Index in From_Tree.Units.all'Range loop
            if Find (This.all, From_Tree.Units.all (Index).Unit) /= null then
               raise Use_Error;
            end if;
         end loop;

         To_Node.Next := Add_Node (To_Node.Next, From_Tree.Self);

         for Index in From_Tree.Last_Nodes.all'Range loop
            This.Last_Nodes := Add_Node
              (This.Last_Nodes, From_Tree.Last_Nodes.all (Index));
         end loop;

         Deallocate (From_Tree.Last_Nodes);

         for Index in From_Tree.Units.all'Range loop
            This.Units := Add_Node_Ordered
              (This.Units, From_Tree.Units.all (Index).Node);
         end loop;

         Deallocate (From_Tree.Units);
         From_Tree := null;
      end Add_Thread;

      ---------------
      -- Add_Child --
      ---------------

      function Add_Child
        (This      : in Tree_Node_Access;
         Node      : in Tree_Node_Access;
         Spec_Unit : in Compilation_Unit;
         Body_Unit : in Compilation_Unit)
         return Tree_Node_Access
      is
         New_Node : Tree_Node_Access := new Tree_Node;
      begin
         if This.Prev /= null then
            --  not root
            raise Use_Error;
         end if;

         New_Node.Unit      := Spec_Unit;
         New_Node.Unit_Body := Body_Unit;

         if Node = null then
            This.Next     := Add_Node (This.Next, New_Node.Self);
            New_Node.Prev := This.Self;
         else
            Node.Next     := Add_Node (Node.Next, New_Node.Self);
            New_Node.Prev := Node.Self;
         end if;

         This.Units := Add_Node_Ordered (This.Units, New_Node.Self);
         return New_Node;
      end Add_Child;

      ----------------
      -- Glue_Nodes --
      ----------------

      procedure Glue_Nodes
        (This    : in Tree_Node_Access;
         Node    : in Tree_Node_Access;
         To_Node : in Tree_Node_Access)
      is
      begin
         if This.Prev /= null then
            --  not root
            raise Use_Error;
         end if;

         Node.Next := Add_Node (Node.Next, To_Node.Self);
      end Glue_Nodes;

      -----------
      -- Clear --
      -----------

      procedure Deallocate is
        new Ada.Unchecked_Deallocation
          (Tree_Node, Tree_Node_Access);

      procedure Clear
        (This : in out Tree_Node)
      is
         Node : Tree_Node_Access;
      begin
         if This.Next /= null then
            for Index in This.Next.all'Range loop
               Node := This.Next.all (Index);
               Clear (Node.all);
               Deallocate (Node);
            end loop;

            Deallocate (This.Next);
         end if;

         Deallocate (This.Last_Nodes);
         Deallocate (This.Units);
         Deallocate (This.Circular);
         Deallocate (This.Missing);
         Deallocate (This.Inconsistent);
      end Clear;

      -----------
      -- Check --
      -----------

      procedure Check
        (This  : in Tree_Node_Access;
         Order : in Orders)
      is
         Node                : Tree_Node_Access := This;
         Kinds, Parent_Kinds : Unit_Kinds;
         Perent_Unit         : Compilation_Unit;
      begin
         if Order = Ascending then
            if not Is_Nil (Node.Unit) then
               Kinds := Unit_Kind (Node.Unit);

               --  inconsistent
               if Node.Consistent then
                  if not Is_Inconsistent (Node.Unit) then
                     Node.Consistent := False;

                     if Is_Source_Changed (Node.Unit) then
                        Node.Inconsistent := Append
                          (Node.Inconsistent,
                           (Nil_Compilation_Unit, Node.Unit));
                     else
                        if not Is_Nil (Node.Prev.Unit) then
                           Node.Inconsistent := Append
                             (Node.Inconsistent,
                              (Node.Prev.Unit, Node.Unit));
                        else
                           Node.Inconsistent := Append
                             (Node.Inconsistent,
                              (Node.Unit, Node.Unit));
                        end if;
                     end if;
                  end if;

                  if not Is_Nil (Node.Unit_Body) then
                     if not Node.Consistent then
                        Node.Inconsistent := Append
                          (Node.Inconsistent,
                           (Node.Unit, Node.Unit_Body));
                     else
                        if not Is_Inconsistent (Node.Unit_Body) then
                           if Is_Source_Changed (Node.Unit_Body) then
                              Node.Inconsistent := Append
                                (Node.Inconsistent,
                                 (Nil_Compilation_Unit, Node.Unit_Body));
                           else
                              Node.Inconsistent := Append
                                (Node.Inconsistent,
                                 (Node.Unit_Body, Node.Unit_Body));
                           end if;
                        end if;
                     end if;
                  end if;

                  if not Node.Consistent
                    and then Node.Next /= null
                  then
                     for Index in Node.Next.all'Range loop
                        Node.Inconsistent := Set_Inconsistent
                          (Node.Next.all (Index), Node.Inconsistent, Order);
                     end loop;
                  end if;
               end if;

               if Node.Prev /= null
                 and then not Is_Nil (Node.Prev.Unit)
               then
                  Perent_Unit  := Node.Prev.Unit;
                  Parent_Kinds := Unit_Kind (Perent_Unit);

                  --  missing
                  if Kinds in A_Procedure .. A_Generic_Package_Renaming then
                     if Parent_Kinds = A_Nonexistent_Declaration then
                        Node.Missing := Append
                          (Node.Missing, (Node.Unit, Perent_Unit));
                     end if;

                  elsif Kinds in A_Library_Unit_Body then
                     if Parent_Kinds = A_Nonexistent_Body then
                        Node.Missing := Append
                          (Node.Missing, (Node.Unit, Perent_Unit));
                     end if;
                  end if;

                  if not Is_Nil (Node.Unit_Body) then
                     if not Is_Nil (Node.Prev.Unit_Body) then
                        Perent_Unit := Node.Prev.Unit_Body;
                     end if;

                     Parent_Kinds := Unit_Kind (Perent_Unit);

                     if Kinds = A_Nonexistent_Declaration
                       or else Kinds = A_Nonexistent_Declaration
                     then
                        Node.Missing := Append
                          (Node.Missing, (Node.Unit_Body, Node.Unit));
                     end if;

                     if Parent_Kinds = A_Nonexistent_Body then
                        Node.Missing := Append
                          (Node.Missing, (Node.Unit_Body, Perent_Unit));
                     end if;
                  end if;
               end if;
            end if;

            if Node.Next /= null then
               for Index in Node.Next.all'Range loop
                  Check (Node.Next.all (Index), Order);
               end loop;
            end if;
         else
            null;
         end if;
      end Check;

      ---------------------------
      -- Generate_Relationship --
      ---------------------------

      function Generate_Relationship
        (This       : in Tree_Node_Access;
         Limit_List : in Utils.Compilation_Unit_List_Access;
         List_Last  : in ASIS_Integer;
         Order      : in Orders)
         return Relationship
      is
         Consistent_List   : Compilation_Unit_List_Access := null;
         Inconsistent_List : Compilation_Unit_List_Access := null;
         Missing_List      : Compilation_Unit_List_Access := null;
         Circular_List     : Compilation_Unit_List_Access := null;

         Consistent_Length   : Asis.ASIS_Natural := 0;
         Inconsistent_Length : Asis.ASIS_Natural := 0;
         Missing_Length      : Asis.ASIS_Natural := 0;
         Circular_Length     : Asis.ASIS_Natural := 0;

         -- Genegate_Inconsistent --
         procedure Genegate_Inconsistent
           (Node : in Tree_Node_Access)
         is
         begin
            if Node.Inconsistent /= null
              and then not Node.Inconsistent_Added
            then
               Node.Inconsistent_Added := True;

               Inconsistent_List := Append
                 (Inconsistent_List, Node.Inconsistent.all);
            end if;
         end Genegate_Inconsistent;

         -- Genegate_Circular --
         procedure Genegate_Circular
           (Node : in Tree_Node_Access)
         is
         begin
            if Node.Circular /= null
              and then not Node.Circular_Added
            then
               Node.Circular_Added := True;

               for Index in Node.Circular.all'Range loop
                  Circular_List := Append
                    (Circular_List, Node.Circular.all (Index));

                  if Index < Node.Circular.all'Last then
                     Circular_List := Append
                       (Circular_List, Node.Circular.all (Index + 1));
                  else
                     Circular_List := Append
                       (Circular_List, Node.Circular.all (1));
                  end if;
               end loop;
            end if;
         end Genegate_Circular;

         -- Genegate_Missing --
         procedure Genegate_Missing
           (Node : in Tree_Node_Access)
         is
         begin
            if Node.Missing /= null
              and then not Node.Missing_Added
            then
               Node.Missing_Added := True;

               Missing_List := Append (Missing_List, Node.Missing.all);
            end if;
         end Genegate_Missing;

         -- Process_Asc --
         procedure Process_Asc
           (Node : in Tree_Node_Access)
         is
            Internal_Node : Tree_Node_Access := Node;
         begin
            while Internal_Node /= null loop
               if not Is_Empty (Internal_Node.all) then
                  if Internal_Node.Added then
                     return;
                  end if;

                  Internal_Node.Added := True;

                  if Internal_Node.Consistent then
                     Consistent_List := Append
                       (Consistent_List, Internal_Node.Unit);
                  end if;

                  Genegate_Inconsistent (Internal_Node);
                  Genegate_Missing      (Internal_Node);
                  Genegate_Circular     (Internal_Node);
               end if;

               Internal_Node := Internal_Node.Prev;
            end loop;
         end Process_Asc;

         -- Process_Dsc --
         procedure Process_Dsc
           (Node : in Tree_Node_Access)
         is
            -- Add_To_Consistent --
            procedure Add_To_Consistent
               (Unit : in Compilation_Unit)
            is
            begin
               if Limit_List /= null then
                  if In_List (Limit_List, List_Last, Unit) then
                     Consistent_List := Append (Consistent_List, Unit);
                  end if;
               else
                  Consistent_List := Append (Consistent_List, Unit);
               end if;
            end Add_To_Consistent;
         begin
            if Node.Added then
               return;
            end if;

            Node.Added := True;

            if Node.Consistent then
               Add_To_Consistent (Node.Unit);

               if not Is_Nil (Node.Unit_Body)
                 and then Is_Inconsistent (Node.Unit_Body)
               then
                  Add_To_Consistent (Node.Unit_Body);
               end if;
            end if;

            Genegate_Inconsistent (Node);
            Genegate_Missing      (Node);
            Genegate_Circular     (Node);

            if Node.Next /= null then
               for Index in Node.Next.all'Range loop
                  Process_Dsc (Node.Next.all (Index));
               end loop;
            end if;
         end Process_Dsc;

      begin
         if Order = Ascending then
            if Is_Empty (This.all) then
               return Nil_Relationship;
            end if;

            declare
               Lasts : Tree_Node_Array_Access := This.Last_Nodes;
            begin
               for Index in Lasts.all'Range loop
                  Process_Asc (Lasts.all (Index));
               end loop;
            end;
         else
            if not Is_Nil (This.Unit) then
               Process_Dsc (This);
            else
               if This.Next = null then
                  return Nil_Relationship;
               end if;

               for Index in This.Next.all'Range loop
                  Process_Dsc (This.Next.all (Index));
               end loop;
            end if;
         end if;

         if Consistent_List /= null then
            Consistent_Length := Consistent_List.all'Length;
         end if;

         if Inconsistent_List /= null then
            Inconsistent_Length := Inconsistent_List.all'Length;
         end if;

         if Missing_List /= null then
            Missing_Length := Missing_List.all'Length;
         end if;

         if Circular_List /= null then
            Circular_Length := Circular_List.all'Length;
         end if;

         declare
            Result : Relationship
              (Consistent_Length, Inconsistent_Length,
               Missing_Length, Circular_Length);
         begin
            if Consistent_List /= null then
               Result.Consistent := Consistent_List.all;
            end if;

            if Inconsistent_List /= null then
               Result.Inconsistent := Inconsistent_List.all;
            end if;

            if Missing_List /= null then
               Result.Missing := Missing_List.all;
            end if;

            if Circular_List /= null then
               Result.Circular := Circular_List.all;
            end if;

            Deallocate (Consistent_List);
            Deallocate (Inconsistent_List);
            Deallocate (Missing_List);
            Deallocate (Circular_List);

            return Result;
         end;

      exception
         when others =>
            Deallocate (Consistent_List);
            Deallocate (Inconsistent_List);
            Deallocate (Missing_List);
            Deallocate (Circular_List);
            raise;
      end Generate_Relationship;

      --------------
      -- Finalize --
      --------------

      procedure Finalize
        (This : in out Tree_Node)
      is
      begin
         Clear (This);
      end Finalize;

      ----------
      -- Find --
      ----------

      function Find
        (This : in Tree_Node;
         Unit : in Compilation_Unit)
         return Tree_Node_Access
      is
         Index : aliased Positive;
      begin
         if This.Units = null then
            return null;
         end if;

         if Find
           (This.Units, Unit, 1, This.Units.all'Last, Index'Unchecked_Access)
         then
            return This.Units.all (Index).Node;
         else
            return null;
         end if;
      end Find;

      ----------------------
      -- Set_Inconsistent --
      ----------------------

      function Set_Inconsistent
        (This  : in Tree_Node_Access;
         List  : in Compilation_Unit_List_Access;
         Order : in Orders)
         return Compilation_Unit_List_Access
      is
         Node                : Tree_Node_Access := This;
         Kinds, Parent_Kinds : Unit_Kinds;

         Result : Compilation_Unit_List_Access := List;
      begin
         if Order = Ascending then
            if not Is_Nil (Node.Unit) then
               Kinds        := Unit_Kind (Node.Unit);
               Parent_Kinds := Unit_Kind (Node.Prev.Unit);

               if Kinds in A_Procedure .. A_Generic_Package_Renaming
                 and then Parent_Kinds in
                   A_Procedure .. A_Generic_Package_Renaming
               then
                  Node.Consistent := False;
                  Result := Append (Result, (Node.Prev.Unit, Node.Unit));
               end if;

               if not Is_Nil (Node.Unit_Body) then
                  if not Node.Consistent then
                     Result := Append
                       (Result, (Node.Unit, Node.Unit_Body));
                  else
                     if not Is_Inconsistent (Node.Unit_Body) then
                        if Is_Source_Changed (Node.Unit_Body) then
                           Result := Append
                             (Result, (Nil_Compilation_Unit, Node.Unit_Body));
                        else
                           Result := Append
                             (Result, (Node.Unit_Body, Node.Unit_Body));
                        end if;
                     end if;
                  end if;
               end if;

               if not Node.Consistent
                 and then Node.Next /= null
               then
                  for Index in Node.Next.all'Range loop
                     Result := Set_Inconsistent
                       (Node.Next.all (Index), Result, Order);
                  end loop;
               end if;
            end if;

         else
            null;
         end if;

         return Result;
      end Set_Inconsistent;

      --------------
      -- Add_Node --
      --------------

      function Add_Node
        (List : in Tree_Node_Array_Access;
         Node : in Tree_Node_Access)
         return Tree_Node_Array_Access
      is
         Array_Access : Tree_Node_Array_Access := List;
      begin
         if Array_Access = null then
            Array_Access := new Tree_Node_Array (1 .. 1);
         else
            declare
               Tmp_Array : Tree_Node_Array_Access :=
                 new Tree_Node_Array (1 .. Array_Access.all'Last + 1);
            begin
               Tmp_Array (1 .. Array_Access.all'Last) := Array_Access.all;
               Deallocate (Array_Access);
               Array_Access := Tmp_Array;
            end;
         end if;

         Array_Access.all (Array_Access.all'Last) := Node;
         return Array_Access;
      end Add_Node;

      ----------------------
      -- Add_Node_Ordered --
      ----------------------

      function Add_Node_Ordered
        (List : in Unit_Node_Array_Access;
         Node : in Tree_Node_Access)
         return Unit_Node_Array_Access
      is
         Array_Access : Unit_Node_Array_Access := List;
         Index : aliased Positive;

         procedure Process
           (Unit : Compilation_Unit)
         is
         begin
            if Array_Access = null then
               Array_Access := new Unit_Node_Array (1 .. 1);
               Array_Access.all (1) := (Unit, Node);
            else
               if Find
                 (Array_Access, Unit,
                  1, Array_Access.all'Last, Index'Unchecked_Access)
               then
                  raise Use_Error;
               end if;

               declare
                  Tmp_Array : Unit_Node_Array_Access :=
                    new Unit_Node_Array (1 .. Array_Access.all'Last + 1);
               begin
                  Tmp_Array (1 .. Index - 1) :=
                    Array_Access.all (1 .. Index - 1);

                  Tmp_Array (Index) := (Unit, Node);

                  Tmp_Array (Index + 1 .. Tmp_Array.all'Last) :=
                    Array_Access.all (Index .. Array_Access.all'Last);

                  Deallocate (Array_Access);
                  Array_Access := Tmp_Array;
               end;
            end if;
         end Process;

      begin
         Process (Node.Unit);

         if not Is_Nil (Node.Unit_Body) then
            Process (Node.Unit_Body);
         end if;

         return Array_Access;
      end Add_Node_Ordered;

      ----------
      -- Find --
      ----------

      function Find
        (List  : in Unit_Node_Array_Access;
         Unit  : in Compilation_Unit;
         From  : in Positive;
         To    : in Positive;
         Index : in Positive_Access)
         return Boolean
      is
         L, H, I : Natural;
         C       : Integer;
         Result  : Boolean := False;

      begin
         L := From;
         H := To;

         while L <= H loop
            I := (L + H) / 2;
            C := Compare (List.all (I).Unit, Unit);

            if C < 0 then
               L := I + 1;
            else
               H := I - 1;

               if C = 0 then
                  Result := True;
                  L      := I;
               end if;
            end if;
         end loop;

         Index.all := L;
         return Result;
      end Find;

      -------------
      -- Compare --
      -------------

      function Compare
        (Left  : in Compilation_Unit;
         Right : in Compilation_Unit)
         return Integer
      is
         use Asis;
         use System;
      begin
         if Left.all'Address < Right.all'Address then
            return -1;

         elsif Left.all'Address > Right.all'Address then
            return 1;

         else
            return 0;
         end if;
      end Compare;

      -------------
      -- In_List --
      -------------

      function In_List
        (List : in Compilation_Unit_List_Access;
         Last : in ASIS_Integer;
         Unit : in Compilation_Unit)
         return Boolean
      is
      begin
         for Index in 1 .. Last loop
            if Asis.Compilation_Units.Is_Identical (List (Index), Unit) then
               return True;
            end if;
         end loop;

         return False;
      end In_List;

      ----------------------
      -- Remove_From_List --
      ----------------------

      procedure Remove_From_List
        (List : in out Compilation_Unit_List;
         From : in     List_Index;
         Unit : in     Compilation_Unit)
      is
      begin
         for Index in From .. List'Last loop
            if Is_Identical (List (Index), Unit) then
               List (Index) := Nil_Compilation_Unit;
               return;
            end if;
         end loop;
      end Remove_From_List;

      ------------
      -- Append --
      ------------

      function Append
        (List : in Compilation_Unit_List_Access;
         Unit : in Compilation_Unit)
         return Compilation_Unit_List_Access
      is
         Result : Compilation_Unit_List_Access := List;
      begin
         if Result = null then
            Result := new Compilation_Unit_List (1 .. 1);
         else
            declare
               Tmp_Array : Compilation_Unit_List_Access :=
                 new Compilation_Unit_List (1 .. Result.all'Last + 1);
            begin
               Tmp_Array (1 .. Result.all'Last) := Result.all;
               Deallocate (Result);
               Result := Tmp_Array;
            end;
         end if;

         Result.all (Result.all'Last) := Unit;
         return Result;
      end Append;

      -- Append --
      function Append
        (List  : in Compilation_Unit_List_Access;
         Units : in Compilation_Unit_List)
         return Compilation_Unit_List_Access
      is
         Result : Compilation_Unit_List_Access := List;
      begin
         if Result = null then
            Result := new Compilation_Unit_List (1 .. Units'Last);
            Result.all := Units;
         else
            declare
               Tmp_Array : Compilation_Unit_List_Access :=
                 new Compilation_Unit_List
                   (1 .. Result.all'Last + Units'Length);
            begin
               Tmp_Array (1 .. Result.all'Last) := Result.all;
               Tmp_Array (Result.all'Last + 1 .. Tmp_Array.all'Last) := Units;
               Deallocate (Result);
               Result := Tmp_Array;
            end;
         end if;

         return Result;
      end Append;

      ---------------------
      -- Is_Inconsistent --
      ---------------------

      function Is_Inconsistent
        (Unit : in Compilation_Unit)
         return Boolean
      is
      begin
         return True;
      end Is_Inconsistent;

      -----------------------
      -- Is_Source_Changed --
      -----------------------

      function Is_Source_Changed
        (Unit : in Compilation_Unit)
         return Boolean
      is
      begin
         return False;
      end Is_Source_Changed;

   end Utils;

end Asis.Compilation_Units.Relations;

------------------------------------------------------------------------------
--  Copyright (c) 2006, Maxim Reznik
--  All rights reserved.
--
--  Redistribution and use in source and binary forms, with or without
--  modification, are permitted provided that the following conditions are met:
--
--     * Redistributions of source code must retain the above copyright notice,
--     * this list of conditions and the following disclaimer.
--     * Redistributions in binary form must reproduce the above copyright
--     * notice, this list of conditions and the following disclaimer in the
--     * documentation and/or other materials provided with the distribution.
--
--  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
--  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
--  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
--  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
--  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
--  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
--  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
--  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
--  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
--  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
--  POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------