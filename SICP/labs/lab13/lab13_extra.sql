CREATE TABLE pairs AS
  WITH 
    nums(n) AS (
      SELECT 1 UNION
      SELECT n + 1 FROM nums WHERE n < 42
    )
  SELECT a.n, b.n FROM nums AS a, nums AS b WHERE a.n + b.n = 42 AND a.n < b.n;
