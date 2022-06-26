% oktay ozel
% 2019400108
% compiling: yes
% complete: yes
:- ['cmpecraft.pro'].

:- init_from_map.


%helper functions

abs(X, Y) :- X < 0, Y is -X.
abs(X, X) :- X >= 0.

% 10 points
% manhattan distance(+List1, +List2, -Distance)

manhattan_distance([X1|Y1], [X2|Y2], D) :- D is abs(X2-X1) + abs(Y2-Y1).

% 10 points
% minimum_of_list(+List, -Minimum) :- .

minimum_of_list([H], H).
minimum_of_list([Head|Tail], Minimum) :- minimum_of_list(Tail, TailMin),
                                            Minimum is min(Head, TailMin).



% 10 points
% find_nearest_type(+State, +ObjectType, -ObjKey, -Object, -Distance) :- .
%find_nearest_type(+State, +ObjectType, -ObjKey, -Object, -Distance) :-
%takes all manhattan distances and uses the changed version of minimum of lists function




minnnn(Head, Tailmin, ObjectDict, Ax, Ay, Ans):-
    get_dict(Head, ObjectDict, ObjHead),
    get_dict(x, ObjHead, Headx),
    get_dict(y, ObjHead, Heady),

    get_dict(Tailmin, ObjectDict, ObjTail),
    get_dict(x, ObjTail, Tailx),
    get_dict(y, ObjTail, Taily),

    manhattan_distance([Ax|Ay], [Headx|Heady], HeadD),
    manhattan_distance([Ax|Ay], [Tailx|Taily], TailD),

    HeadD>TailD -> Ans is Tailmin ; Ans is Head.

%changed version of minimum of lists
mol([H],_, _,_, H).
mol([Head|Tail],ObjectDict,Ax,Ay, Minimum) :- mol(Tail, ObjectDict,Ax,Ay, TailMin),
                                              minnnn(Head,TailMin ,ObjectDict, Ax, Ay ,Ans),
                                              Minimum is Ans.

check(A, ObjectType, ObjectDict):-
    get_dict(A,ObjectDict,Objj),
    get_dict(type,Objj,Otype),
    ObjectType=Otype.


find_nearest_type(State, ObjectType, Key, Obj, Dist) :-
    State = [AgentDict, ObjectDict, _],
    get_dict(x, AgentDict, X),
    get_dict(y, AgentDict, Y),
    dict_pairs(ObjectDict,_,P),
    pairs_values(P,_),
    dict_keys(P,Keys),
    findall(Element,check(Element,ObjectType,ObjectDict) ,FilteredKeys),
    mol( FilteredKeys, ObjectDict, X,Y, Key),
    get_dict(Key, ObjectDict, Ansss),
    get_dict(x, Ansss, Ax),
    get_dict(y, Ansss, Ay),
    manhattan_distance([X|Y], [Ax|Ay], Ress),
    Dist is Ress,
    State = [AgentDict, ObjectDict, _],
    get_dict(Key,ObjectDict, Obj).


% 10 points
% navigate_to(+State, +X, +Y, -ActionList, +DepthLimit) :- .
% does not anything special. Just takes the difference between axes and adds necessary amount of goleft go right goup and godown

append( [],List, List).
append([Head|Tail], List, [Head|Rest]) :-
    append(Tail, List, Rest).


go_up(X,Ax,[]):- X = Ax.
go_up(X,Ax,Ans):- X > Ax , Axnew is Ax + 1, go_up(X,Axnew,Res), append(Res, [go_right], Ans).

go_down(X,Ax,[]):- X = Ax.
go_down(X,Ax,Ans):- X < Ax , Axnew is Ax - 1, go_down(X, Axnew, Res ), append(Res, [go_left], Ans)  .

go_right(Y,Ay,[]):- Y = Ay.
go_right(Y,Ay,Ans):- Y > Ay, Aynew is Ay + 1,  go_right(Y, Aynew, Res) , append(Res, [go_down], Ans) .

go_left(Y,Ay,[]):- Y=Ay.
go_left(Y,Ay,Ans):- Y < Ay , Aynew is Ay -1 , go_left(Y,Aynew, Res) , append(Res, [go_up], Ans) .

navigate_to(State, X, Y, ActionList, DepthLimit):-
    State = [AgentDict, _, _],
    get_dict(x, AgentDict, Ax),
    get_dict(y, AgentDict, Ay),

    write("test"),

    V1 is X-Ax,
    V2 is Y-Ay,
    abs(V1,V3),
    abs(V2,V4),
    V5 is V3 + V4,

    V5 =< DepthLimit,

    write("test1111"),
   
    (X > Ax ->  (go_up(X,Ax,Ans)) ; ( go_down(X,Ax,Ans)) ) ,

    (Y > Ay ->  (go_right(Y,Ay,Ans1)) ;  (go_left(Y,Ay,Ans1))),

    append(Ans,Ans1, ActionList).


% 10 points
% chop_nearest_tree(+State, -ActionList) :- .
% finds nearest tree by using nearest type and navigates to there and clicks left


click_center(X,Ax,[]):- X = Ax.
click_center(X,Ax,Ans):- X > Ax , Axnew is Ax + 1, click_center(X, Axnew, Res ), append(Res, [left_click_c], Ans) .

chop_nearest_tree(State, ActionList):-
    State = [AgentDict, _, _],
    get_dict(x, AgentDict, Ax),
    get_dict(y, AgentDict, Ay),
    find_nearest_type(State, tree, _, Obj, _),
    get_dict(x,Obj,Aimx),
    get_dict(y,Obj,Aimy),
    V1 is Aimx-Ax,
    V2 is Aimy-Ay,
    abs(V1,V3),
    abs(V2,V4),
    V5 is V3 + V4,
    DepthLimit is V5+15,
    navigate_to(State, Aimx, Aimy, Way, DepthLimit),
    Cnt is -1,
    get_dict(hp,Obj,Aimhp),
    click_center(Aimhp,Cnt,Ans),
    append(Way,Ans, ActionList).

% 10 points
% mine_nearest_stone(+State, -ActionList) :- .
% finds nearest stone by using nearest type and navigates to there and clicks left

mine_nearest_stone(State, ActionList) :- 
    State = [AgentDict, _, _],
    get_dict(x, AgentDict, Ax),
    get_dict(y, AgentDict, Ay),
    find_nearest_type(State, stone, _, Obj, _),
    get_dict(x,Obj,Aimx),
    get_dict(y,Obj,Aimy),
    V1 is Aimx-Ax,
    V2 is Aimy-Ay,
    abs(V1,V3),
    abs(V2,V4),
    V5 is V3 + V4,
    DepthLimit is V5+15,
    navigate_to(State, Aimx, Aimy, Way, DepthLimit),
    Cnt is -1,
    get_dict(hp,Obj,Aimhp),
    click_center(Aimhp,Cnt,Ans),
    append(Way,Ans, ActionList).


% 10 points
% gather_nearest_food(+State, -ActionList) :- .
% finds nearest food by using nearest type and navigates to there and clicks left
gather_nearest_food(State, ActionList) :- 
    State = [AgentDict, _, _],
    get_dict(x, AgentDict, Ax),
    get_dict(y, AgentDict, Ay),
    find_nearest_type(State, food, _, Obj, _),
    get_dict(x,Obj,Aimx),
    get_dict(y,Obj,Aimy),
    V1 is Aimx-Ax,
    V2 is Aimy-Ay,
    abs(V1,V3),
    abs(V2,V4),
    V5 is V3 + V4,
    DepthLimit is V5+15,
    navigate_to(State, Aimx, Aimy, Way, DepthLimit),
    Cnt is -1,
    get_dict(hp,Obj,Aimhp),
    click_center(Aimhp,Cnt,Ans),
    append(Way,Ans, ActionList).




% 10 points
% state(A, O, T), State=[A, O, T], collect_requirements(State, stone_pickaxe,ActionList).
% collect_requirements(+State, +ItemType, -ActionList) :- .
%first gets the dict of required elements and than tries to find necessary elements if they are not in current inventory

return_lack_items(AgentInventory,ItemType, LackItemList):-
    item_info(ItemType, RequirementsDict, _),
    findall((X,Y), ( (get_dict(X,RequirementsDict,Y), \+get_dict(X,AgentInventory,Y)) ;  (get_dict(X,RequirementsDict,Z), get_dict(X,AgentInventory,T) , Z > T , Y is Z-T ) ), LackItemList ).
    

    
call_according_to_type(stick,State,ActionList,_):-
    chop_nearest_tree(State, TempActionList),
    append(TempActionList,[craft_stick],ActionList).

call_according_to_type(log,State,ActionList,_):-
    chop_nearest_tree(State, ActionList).


call_according_to_type(cobblestone,State,ActionList,Y):-
    0 =< Y,
    Y =< 3,
    mine_nearest_stone(State, ActionList).

call_according_to_type(cobblestone,State,ActionList,Y):-
    4 =< Y,
    Y =< 6,
    mine_nearest_stone(State, ActionList1),
    execute_actions(State,ActionList1,State1),
    mine_nearest_stone(State1, ActionList2),
    execute_actions(State1,ActionList2,_),
    append(ActionList1,ActionList2,ActionList).

call_according_to_type(cobblestone,State,ActionList,Y):-

    7 =< Y,

    mine_nearest_stone(State, ActionList1),
    execute_actions(State,ActionList1,State1),
    mine_nearest_stone(State1, ActionList2),
    execute_actions(State1,ActionList2,State2),
    mine_nearest_stone(State2, ActionList3),
    append(ActionList1,ActionList2,TempList),
    append(TempList,ActionList3,ActionList).



collect_item([],[],_).    
collect_item([ItemPair|Tail],ActionList,State):-
    (X,Y) = ItemPair,
    call_according_to_type(X,State,TempActionList,Y),
    write("ANANANAN"),
    execute_actions(State, TempActionList,NewState),
    append(TempActionList,NewActionList,ActionList),
    collect_item(Tail, NewActionList , NewState).
    

collect_requirements(State, ItemType, ActionList) :-
    State = [AgentDict, _, _],
    get_dict(inventory, AgentDict, AgentInventory),
    return_lack_items(AgentInventory, ItemType, LackItemList),
    collect_item(LackItemList, ActionList, State).


% 5 points
% find_castle_location(+State, -XMin, -YMin, -XMax, -YMax) :- .
% creates a list of all doubles first and than eliminates them using findall after that selects the first point

block(stone).
block(tree).
block(bedrock).
block(cobblestone).
block(food).

is_full(X, Y, State) :-
    State = [_, StateDict, _],
    get_dict(_, StateDict, Object),
    get_dict(x, Object, Ox),
    get_dict(y, Object, Oy),
    get_dict(type, Object, Type),
    block(Type),
    X = Ox, Y = Oy.

ccc(X,Y,State):-
    A is X-1,
    B is X,
    C is X+1,
    K is Y-1,
    L is Y,
    M is Y+1,
    height(H),
    width(W),
    A >= 1,
    C =< W,
    K >= 1,
    M =< H,
    \+is_full(A,K,State),
    \+is_full(A,L,State),
    \+is_full(A,M,State),
    \+is_full(B,K,State),
    \+is_full(B,L,State),
    \+is_full(B,M,State),
    \+is_full(C,K,State),
    \+is_full(C,L,State),
    \+is_full(C,M,State).


list_pairs(List1, List2, Pairs,State) :-
    findall((X,Y), (member(X, List1), member(Y, List2), ccc(X,Y,State) ), Pairs ).

first([(X,_)|_], X).
second([(_,Y)|_], Y).


find_castle_location(State, XMin, YMin, XMax ,YMax):-
    height(H),
    width(W),
    numlist(1,W,XX),
    numlist(1,H,YY),
    list_pairs(XX,YY,Pairs,State),
    first(Pairs,XXX),
    second(Pairs,YYY),
    XMin is XXX-1,
    YMin is YYY-1,
    XMax is XXX+1,
    YMax is YYY+1.
   







% 15 points
% make_castle(+State, -ActionList) :- .
% determines the necessary amount of cobblestones and than go get them after that determines the castle location and puts cobblestones
make_castle(State, ActionList):-
 
    State = [AgentSon, _, _],
    get_dict( inventory,AgentSon, AgentSonInventory ),

    get_dict(cobblestone, AgentSonInventory , CobblestoneNum) -> L is (9 - CobblestoneNum ); L is 9,


    
    collect_item([(cobblestone,L)], ActionListSon, State),

    execute_actions(State, ActionListSon,NewState3),


    find_castle_location(NewState3, Xmin, Ymin, _ ,_),

    ToX is Xmin + 1,
    ToY is Ymin + 1,
    NewState3 = [AgentDict1, _, _],
    get_dict(x, AgentDict1, AAx),
    get_dict(y, AgentDict1, AAy),
    VV1 is ToX-AAx,
    VV2 is ToY-AAy,
    abs(VV1,VV3),
    abs(VV2,VV4),
    VV5 is VV3 + VV4,
    DepthLimit is VV5+15,
    navigate_to(NewState3, ToX, ToY, BuildActionList, DepthLimit),
    execute_actions(NewState3, BuildActionList,NewState4),
    place_c(NewState4, NS0),
    place_e(NS0,NS1),
    place_n(NS1,NS2),
    place_w(NS2,NS3), 
    place_s(NS3,NS4),
    place_ne(NS4,NS5),
    place_nw(NS5,NS6),
    place_se(NS6,NS7),
    place_sw(NS7,NS8),
    
    PlaceActionList = [place_c,place_e,place_n,place_w,place_s,place_ne,place_nw,place_se,place_sw],
    append(ActionListSon,BuildActionList,ActionListOktay),
    append(ActionListOktay,PlaceActionList,ActionList).


