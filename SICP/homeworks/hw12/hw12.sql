CREATE TABLE parents AS
  SELECT "abraham" AS parent, "barack" AS child UNION
  SELECT "abraham"          , "clinton"         UNION
  SELECT "delano"           , "herbert"         UNION
  SELECT "fillmore"         , "abraham"         UNION
  SELECT "fillmore"         , "delano"          UNION
  SELECT "fillmore"         , "grover"          UNION
  SELECT "eisenhower"       , "fillmore";

CREATE TABLE dogs AS
  SELECT "abraham" AS name, "long" AS fur, 26 AS height UNION
  SELECT "barack"         , "short"      , 52           UNION
  SELECT "clinton"        , "long"       , 47           UNION
  SELECT "delano"         , "long"       , 46           UNION
  SELECT "eisenhower"     , "short"      , 35           UNION
  SELECT "fillmore"       , "curly"      , 32           UNION
  SELECT "grover"         , "short"      , 28           UNION
  SELECT "herbert"        , "curly"      , 31;

CREATE TABLE sizes AS
  SELECT "toy" AS size, 24 AS min, 28 AS max UNION
  SELECT "mini"       , 28       , 35        UNION
  SELECT "medium"     , 35       , 45        UNION
  SELECT "standard"   , 45       , 60;

-------------------------------------------------------------
-- PLEASE DO NOT CHANGE ANY SQL STATEMENTS ABOVE THIS LINE --
-------------------------------------------------------------

-- The size of each dog
CREATE TABLE size_of_dogs AS
  SELECT name, size 
    from dogs, sizes
    where height > min and height <= max;

-- All dogs with parents ordered by decreasing height of their parent
CREATE TABLE by_height AS
  SELECT child
    from dogs, parents
    where name = parent
    order by height desc;

-- Sentences about siblings that are the same size
CREATE TABLE sentences AS
  WITH
    siblings(s1, s2) AS (
      SELECT p1.child, p2.child 
        FROM parents AS p1, parents AS p2
        WHERE p1.parent = p2.parent and p1.child < p2.child
        ORDER BY p1.child
    )
  SELECT s1 || " and " || s2 || " are " || a.size || " siblings"
    FROM siblings, size_of_dogs AS a, size_of_dogs AS b
    WHERE s1 = a.name AND s2 = b.name AND a.size = b.size;

-- Ways to stack 4 dogs to a height of at least 170, ordered by total height
CREATE TABLE stacks AS
  WITH dogs_stacks(dogs_str, last_height, total_height, n) AS (
    SELECT name, height, height, 1 FROM dogs UNION
    SELECT dogs_str || "," || name, height, total_height + height, n + 1
      FROM dogs_stacks, dogs
      WHERE last_height < height and n <= 3
  )
  SELECT dogs_str, total_height
    FROM dogs_stacks
    WHERE total_height > 170 and n = 4
    ORDER BY total_height;
