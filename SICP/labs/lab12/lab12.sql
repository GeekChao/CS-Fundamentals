.read sp17data.sql
.read fa17data.sql

CREATE TABLE obedience AS
  SELECT seven, denero, hilfinger	
    FROM students;

CREATE TABLE smallest_int AS
  SELECT time, smallest
    FROM students
    WHERE smallest > 18
    ORDER BY smallest
    LIMIT 20;

CREATE TABLE greatstudents AS
  SELECT s.date, s.color, s.pet, s.number, f.number
    FROM students AS s, sp17students AS f
    WHERE s.date = f.date and s.color = f.color and s.pet = f.pet;

CREATE TABLE sevens AS
  SELECT s.seven 
    FROM students AS s, checkboxes AS c
    WHERE s.time = c.time and s.number = 7 and c.'7' = 'True';

CREATE TABLE matchmaker AS
  SELECT m1.pet, m1.song, m1.color, m2.color 
    FROM students AS m1, students AS m2
    WHERE m1.time < m2.time AND m1.pet = m2.pet AND m1.song = m2.song;
