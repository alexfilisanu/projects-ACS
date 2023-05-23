
-- =============== DO NOT MODIFY ===================

{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE FlexibleInstances #-}

-- ==================================================

module Tasks where

import Dataset
import Data.List
import Text.Printf
import Data.Array
import Data.Maybe

import Common

type CSV = String
type Value = String
type Row = [Value]
type Table = [Row]
type ColumnName = String

-- Prerequisities
split_by :: Char -> String -> [String]
split_by x = foldr op [""]
  where op char acc
            | char == x = "":acc
            | otherwise = (char:head(acc)):tail(acc)

read_csv :: CSV -> Table
read_csv = (map (split_by ',')) . (split_by '\n')

write_csv :: Table -> CSV
write_csv = (foldr (++) []).
            (intersperse "\n").
            (map (foldr (++) [])).
            (map (intersperse ","))


{-
    TASK SET 1
-}


-- Task 1

compute_average_steps :: Table -> Table
compute_average_steps m = ["Name","Average Number of Steps"] : f (tail m)
        where f = map (\x -> head x : g (tail x) : [])
                where g l = printf "%.2f" ((foldr (\x acc -> acc + (read x) :: Float) 0 l) / 8)


-- Task 2

-- Number of people who have achieved their goal:
get_passed_people_num :: Table -> Int
get_passed_people_num m = f (tail (compute_average_steps m))
        where f l = foldr (\x acc-> if (read (last x) :: Float) >= 125 then (acc + 1) else acc) 0 l


-- Percentage of people who have achieved their:
get_passed_people_percentage :: Table -> Float
get_passed_people_percentage m = fromIntegral(get_passed_people_num m) / fromIntegral(length m - 1)


-- Average number of daily steps
aux_counter_all_steps :: Table -> Float
aux_counter_all_steps m = f (tail(m))
        where f l = foldr (\x acc -> acc + g (tail x)) 0 l
                where g l = foldr (\x acc -> acc + (read x) :: Float) 0 l

get_steps_avg :: Table -> Float
get_steps_avg m = (aux_counter_all_steps m) / fromIntegral(length m - 1)

-- Task 3

get_avg_steps_per_h :: Table -> Table
get_avg_steps_per_h m = ["H10","H11","H12","H13","H14","H15","H16","H17"] : f (tail m) : []
        where f l = map (printf "%.2f") (map (/fromIntegral(length m - 1)) (foldr (\x acc -> zipWith (+)
                (map read (tail x) :: [Float]) acc) [0,0,0,0,0,0,0,0] l))


-- Task 4
aux_activity_range :: Row -> Int -> Int -> Value
aux_activity_range l low high = show (foldr (\x acc -> if (((read x) :: Int) >= low && ((read x) :: Int) < high)
                          then (acc + 1) else acc) 0 l)

get_activ_summary :: Table -> Table
get_activ_summary m = ["column","range1","range2","range3"] : f (map (!!3) m) : f (map (!!4) m) : f (map (!!5) m) : []
        where f l = head l : g (tail l)
                where g l = (aux_activity_range l 0 50) : (aux_activity_range l 50 100) : (aux_activity_range l 100 500) : []


-- Task 5
compareElements :: Ord a => [a] -> [a] -> Ordering
compareElements l1 l2 = if compare ((!!1) l1) ((!!1) l2) == EQ then compare ((!!0) l1) ((!!0) l2)
                  else compare ((!!1) l1) ((!!1) l2)

get_ranking :: Table -> Table
get_ranking m = ["Name","Total Steps"] : f (sortBy compareElements (tail m))
          where f = map (\x -> (!!0) x : (!!1) x : [])


-- Task 6
compareElements2 :: Ord a => [a] -> [a] -> Ordering
compareElements2 l1 l2 = if compare ((!!3) l1) ((!!3) l2) == EQ then compare ((!!0) l1) ((!!0) l2)
                  else compare ((!!3) l1) ((!!3) l2)

get_steps_diff_table :: Table -> Table
get_steps_diff_table m = ["Name","Average first 4h","Average last 4h","Difference"] : (sortBy compareElements2(f (tail m)))
          where f = map (\x -> head x : g (take 4(tail x)) : g (drop 4(tail x)) :
                    (printf "%.2f" (abs ((read (g (take 4(tail x))) :: Float) - (read (g (drop 4(tail x)))) :: Float))) : [])
                  where g l = printf "%.2f" ((foldr (\x acc -> acc + (read x) :: Float) 0 l) / 4)


-- Task 7

-- Applies the given function to all the values
vmap :: (Value -> Value) -> Table -> Table
vmap f m = map (\x -> map f x) m


-- Task 8

-- Applies the given function to all the entries
rmap :: (Row -> Row) -> [String] -> Table -> Table
rmap f s m = s:(map (\x -> f x) (tail m))

get_sleep_total :: Row -> Row
get_sleep_total r = head r : g (tail r) : []
        where g l = printf "%.2f" (foldr (\x acc -> acc + (read x) :: Float) 0 l)


{-
    TASK SET 2
-}

-- Task 1
isNumber :: String -> Bool
isNumber [] = True
isNumber (x:xs) = if (x > '9' || x < '.') then False else isNumber xs

getColumnNumber :: Row -> ColumnName -> Int
getColumnNumber [] _ = 0
getColumnNumber (x:xs) s = if (x /= s) then (+1) (getColumnNumber xs s) else 0

tsort :: ColumnName -> Table -> Table
tsort column table = (head table) : (if isNumber((!!i)(last table)) then sortBy compareNr (tail table) else sortBy compareStr (tail table))
                where i = getColumnNumber (head table) column
                      compareNr l1 l2 = if compare (read ((!!i) l1) :: Double) (read((!!i) l2) :: Double) == EQ
                          then compare ((!!0) l1) ((!!0) l2) else compare (read ((!!i) l1) :: Double) (read((!!i) l2) :: Double)
                      compareStr l1 l2 = if compare ((!!i) l1) ((!!i) l2) == EQ then compare ((!!0) l1) ((!!0) l2)
                          else compare ((!!i) l1) ((!!i) l2)

-- Task 2

vunion :: Table -> Table -> Table
vunion t1 t2 = if head t1 == head t2 then t1 ++ (tail t2) else t1

-- Task 3
makeHunion :: Row -> Row -> Table -> Table -> Table
makeHunion l1 l2 [] [] = []
makeHunion l1 l2 (x:xs) [] = (x++l2):(makeHunion l1 l2 xs [])
makeHunion l1 l2 [] (y:ys) = (y++l1):(makeHunion l1 l2 [] ys)
makeHunion l1 l2 (x:xs) (y:ys) = (x++y):(makeHunion l1 l2 xs ys)

hunion :: Table -> Table -> Table
hunion t1 t2 = makeHunion l1 l2 t1 t2
        where l1 = foldr (\x acc -> "":acc) [] (head t1)
              l2 = foldr (\x acc -> "":acc) [] (head t2)

-- Task 4
-- unesc liniile cu chei identice a 2 tabele, coloana cheii dupa care unesc aparand doar o data (in primul tabel) 
mergeByKey :: Int -> Int -> Table -> Table -> Table -> Table
mergeByKey _ _ _ [] _ = []
mergeByKey i1 i2 l (x:xs) [] = mergeByKey i1 i2 l xs l
mergeByKey i1 i2 l (x:xs) (y:ys) = if ((!!i1)x) == ((!!i2)y) then (x ++ (take i2 y) ++ (drop (i2 + 1) y)) : 
                (mergeByKey i1 i2 l xs l) else (mergeByKey i1 i2 l (x:xs) ys)

-- o sa folosesc aceasta functie pentru a verifica daca exista coloane identice dupa unirea celor 2 tabele 
-- -> returneaza o lista de stringuri cu dublurile (continute in antet) 
getDuplicate :: Row -> Row -> Row -> Row
getDuplicate _ [] _ = []
getDuplicate l (x:xs) [] = getDuplicate l xs l
getDuplicate l (x:xs) (y:ys) = if x == y then y:(getDuplicate l xs l) else (getDuplicate l (x:xs) ys)

-- in listPos am salvat o lista cu indicii stringurilor care se repeta, iar apoi sterg din fiecare linie
-- a tabelului creat elementele de pe pozitiile respective
tjoin :: ColumnName -> Table -> Table -> Table
tjoin key_column t1 t2 = map (\y -> foldr (\x acc -> (take x acc) ++ (drop (x + 1) acc)) y listPos) m
        where m = mergeByKey i1 i2 t2 t1 t2
                where i1 = getColumnNumber (head t1) key_column
                      i2 = getColumnNumber (head t2) key_column
              listPos = map (\x -> getColumnNumber (head m) x) listDup
                    where listDup = getDuplicate (drop len (head m)) (take len (head m)) (drop len (head m))
                          len = length (head t1)

-- Task 5

cartesian :: (Row -> Row -> Row) -> [ColumnName] -> Table -> Table -> Table
cartesian new_row_function new_column_names t1 t2 = new_column_names : (f new_row_function (tail t2) (tail t1) (tail t2))
        where f :: (Row -> Row -> Row) -> Table -> Table -> Table -> Table
              f new_row_function l [] _ = []
              f new_row_function l (x:xs) [] = f new_row_function l xs l
              f new_row_function l (x:xs) (y:ys) = (new_row_function x y):(f new_row_function l (x:xs) ys)  

-- Task 6

projection :: [ColumnName] -> Table -> Table
projection columns_to_extract t = map (\y -> foldr (\x acc -> ((!!x)y) : acc) [] listPos) t
        where listPos = map (\x -> getColumnNumber (head t) x) columns_to_extract

-- Task 7

filterTable :: (Value -> Bool) -> ColumnName -> Table -> Table
filterTable condition key_column t = head t : (f condition i (tail t))
        where i = getColumnNumber (head t) key_column
              f :: (Value -> Bool) -> Int -> Table -> Table
              f condition i t = filter (\x -> condition ((!!i)x)) t

-- Task 8 TO_DO




{-
    TASK SET 3
-}


-- 3.1

data Query =
    FromTable Table
    | AsList String Query 
    | Sort String Query
    | ValueMap (Value -> Value) Query
    | RowMap (Row -> Row) [String] Query
    | VUnion Query Query
    | HUnion Query Query
    | TableJoin String Query Query
    | Cartesian (Row -> Row -> Row) [String] Query Query
    | Projection [String] Query
    | forall a. FEval a => Filter (FilterCondition a) Query
    | Graph EdgeOp Query -- 3.5

makeTable :: QResult -> Table
makeTable (List l) = [l]
makeTable (Table t) = t 

instance Show QResult where
    show (List l) = show l
    show (Table t) = show t

class Eval a where
    eval :: a -> QResult

instance Eval Query where
    eval (FromTable t) = Table t
    eval (AsList colname query) = List (map (!!(getColumnNumber (head t) colname)) (tail t))
                where t = makeTable(eval query)
    eval (Sort colname query) = Table (tsort colname t)
                where t = makeTable(eval query)
    eval (ValueMap op query) = Table (vmap op t)
                where t = makeTable(eval query)
    eval (RowMap op colnames query) = Table (rmap op colnames t)
                where t = makeTable(eval query)
    eval (VUnion query1 query2) = Table (vunion t1 t2)
                where t1 = makeTable(eval query1)
                      t2 = makeTable(eval query2)
    eval (HUnion query1 query2) = Table (hunion t1 t2)
                where t1 = makeTable(eval query1)
                      t2 = makeTable(eval query2)
    eval (TableJoin colname query1 query2) = Table (tjoin colname t1 t2)
                where t1 = makeTable(eval query1)
                      t2 = makeTable(eval query2)
    eval (Cartesian op colnames query1 query2) = Table (cartesian op colnames t1 t2)
                where t1 = makeTable(eval query1)
                      t2 = makeTable(eval query2)
    eval (Projection colnames query) = Table (projection colnames t)
                where t = makeTable(eval query)
    eval (Filter cond query) = Table ((head t) : f(tail t))
                where t = makeTable(eval query)
                      f :: Table -> Table
                      f m = filter (feval (head t) cond) m
    eval (Graph edgeOp query) = Table (["From","To","Value"] : (f edgeOp (tail t) (tail (tail t))))
                where t = makeTable(eval query)
                      f :: EdgeOp -> Table -> Table -> Table
                      f edgeOp [x] _ = []
                      f edgeOp (x:xs) [] = f edgeOp xs (tail xs) 
                      f edgeOp (x:xs) (y:ys) = if (edgeOp x y == Nothing) then (f edgeOp (x:xs) ys) 
                              else (if (head x) < (head y) then (((head x) : (head y) : (fromJust (edgeOp x y)) : []):(f edgeOp (x:xs) ys))
                                                        else ((head y) : (head x) : (fromJust (edgeOp x y)) : []):(f edgeOp (x:xs) ys))
-- 3.2 & 3.3

type FilterOp = Row -> Bool

data FilterCondition a =
    Eq String a |
    Lt String a |
    Gt String a |
    In String [a] |
    FNot (FilterCondition a) |
    FieldEq String String

class FEval a where
    feval :: [String] -> (FilterCondition a) -> FilterOp

instance FEval Float where
    feval thead (Eq colname ref) = \x -> (read((!!i)x) :: Float) == ref
                where i = getColumnNumber thead colname
    feval thead (Lt colname ref) = \x -> (read((!!i)x) :: Float) < ref
                where i = getColumnNumber thead colname
    feval thead (Gt colname ref) = \x -> (read((!!i)x) :: Float) > ref
                where i = getColumnNumber thead colname
    feval thead (In colname list) = \x -> searchFloat list (read((!!i)x) :: Float)
                where i = getColumnNumber thead colname
                      searchFloat :: [Float] -> Float -> Bool
                      searchFloat [] f = False
                      searchFloat (x:xs) f = if (x == f) then True else searchFloat xs f
    feval thead (FNot cond) = \x -> not((feval thead cond) x)
    feval thead (FieldEq colname1 colname2) = \x -> (read((!!i1)x) :: Float) == (read((!!i2)x) :: Float) 
                where i1 = getColumnNumber thead colname1
                      i2 = getColumnNumber thead colname2 

instance FEval String where
    feval thead (Eq colname ref) = \x -> (!!i)x == ref
                where i = getColumnNumber thead colname
    feval thead (Lt colname ref) = \x -> (!!i)x < ref
                where i = getColumnNumber thead colname
    feval thead (Gt colname ref) = \x -> (!!i)x > ref
                where i = getColumnNumber thead colname
    -- daca getColumnNumber returneaza lungimea listei -> elementul cautat nu e in lista 
    feval thead (In colname list) = \x -> getColumnNumber list ((!!i)x) /= length list
                where i = getColumnNumber thead colname
    feval thead (FNot cond) = \x -> not((feval thead cond) x)
    feval thead (FieldEq colname1 colname2) = \x -> (!!i1)x == (!!i2)x 
                where i1 = getColumnNumber thead colname1
                      i2 = getColumnNumber thead colname2 
-- -- 3.4

-- where EdgeOp is defined:
type EdgeOp = Row -> Row -> Maybe Value


-- -- 3.5

edgeOp :: EdgeOp
edgeOp l1 l2 = if res >= 5 then Just (show res) else Nothing
        where res = f (tail l1) (tail l2)
                where f :: Row -> Row -> Integer
                      f [] _ = 0
                      f (x:xs) (y:ys) = if x == y then 1 + (f xs ys) else (f xs ys)

similarities_query :: Query
similarities_query = Filter (FNot (Eq "From" "")) (Sort "Value" (Graph edgeOp (FromTable Dataset.eight_hours)))

-- -- 3.6 (Typos)

correct_table :: String -> Table -> Table -> Table
correct_table col csv1 csv2 = (head csv1) : (fixTable i t ref)
        where i = getColumnNumber (head csv1) col 
              t = tail csv1
              ref = tail (projection (col:[]) csv2)

fixTable :: Int -> Table -> Table -> Table
fixTable _ [] _ = []
fixTable i (x:xs) l = (replace i x (getBest ((!!i)x) l "" 100)) : (fixTable i xs l)  

getBest :: Value -> Table -> Value -> Int -> Value
getBest x [] res max = res
getBest x (y:ys) res max = if (editDistance x (head y)) < max 
        then (getBest x ys (head y) (editDistance x (head y))) else (getBest x ys res max)  

replace :: Int -> Row -> Value -> Row
replace _ [] _ = []
replace i (x:xs) val = if (i == 0) then val:xs else replace (i - 1) xs val  


-- https://wiki.haskell.org/Edit_distance
editDistance :: Eq a => [a] -> [a] -> Int
editDistance xs ys = table ! (m,n)
        where
        (m,n) = (length xs, length ys)
        x     = array (1,m) (zip [1..] xs)
        y     = array (1,n) (zip [1..] ys)
        
        table :: Array (Int,Int) Int
        table = array bnds [(ij, dist ij) | ij <- range bnds]
        bnds  = ((0,0),(m,n))
        
        dist (0,j) = j
        dist (i,0) = i
        dist (i,j) = minimum [table ! (i-1,j) + 1, table ! (i,j-1) + 1,
                if x ! i == y ! j then table ! (i-1,j-1) else 1 + table ! (i-1,j-1)]

