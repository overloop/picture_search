CREATE TABLE color (
  color_id integer primary key autoincrement,
  file_id integer ,
  h integer ,
  s integer ,
  l integer
)

CREATE TABLE directory (
  directory_id integer primary key autoincrement,
  path text,
  subdirs integer ,
  user integer
)

CREATE TABLE file (
  file_id integer primary key autoincrement,
  directory_id integer,
  path text,
  preview blob
)

SELECT directory.path,file.path,preview FROM file JOIN directory 
JOIN color 
WHERE file.directory_id=directory.directory_id AND 
color.file_id=file.file_id AND 
(abs(h-?)*2+abs(s-?)+abs(l-?))<? 
GROUP BY file.file_id