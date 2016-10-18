USE CS143;

/* OUTPUT FROM BELOW QUERIES 

mysql> INSERT INTO MovieGenre VALUES (9090909090, 'Comedy');
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO MovieDirector VALUES (9090909090, 1234);                             
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieDirector`, CONSTRAINT `MovieDirector_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO MovieDirector VALUES (1234, 9090909090);                             
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieDirector`, CONSTRAINT `MovieDirector_ibfk_2` FOREIGN KEY (`did`) REFERENCES `Director` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO MovieActor VALUES (1234, 9090909090, "James Bond");                  
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieActor`, CONSTRAINT `MovieActor_ibfk_2` FOREIGN KEY (`aid`) REFERENCES `Actor` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO MovieActor VALUES (9090909090, 1234, "James Bond");                  
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`MovieActor`, CONSTRAINT `MovieActor_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO Review VALUES ('asdf', '1960-07-15 23:59:59', 9090909090, 10, "James 
Bond");                                                                                 
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`CS143`.`Review`, CONSTRAINT `Review_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`) ON DELETE CASCADE)
mysql> INSERT INTO Movie VALUES (1, 'Hello', 1990, 'R', 'Max Co'), (1, 'Goodbye', 1991, 
'PG-13', 'Ryan Co');                                                                    
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'
mysql> INSERT INTO Actor VALUES (1, 'Chern', 'Max', 'Male', '1975-05-25', '0000-00-00'),
 (1, 'Peterman', 'Ryan', 'Male', '1966-09-17', '0000-00-00');                           
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'
mysql> INSERT INTO Director VALUES (1, 'Jameroni', 'Pinto', '1975-05-25', '0000-00-00'), (1, 'Abidi', 'James', '1966-09-17', '0000-00-00');                                     
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY' */

/* PRIMARY KEY CONSTRAINTS */
/* ALL of these duplicates an ID among different tuples, which is not
allowed in the primary key constraint */
INSERT INTO Movie 
VALUES (1, 'Hello', 1990, 'R', 'Max Co') 
       (1, 'Goodbye', 1991, 'PG-13', 'Ryan Co');

INSERT INTO Actor
VALUES (1, 'Chern', 'Max', 'Male', '1975-05-25', '0000-00-00')
       (1, 'Peterman', 'Ryan', 'Male', '1966-09-17', '0000-00-00');

INSERT INTO Director
VALUES (1, 'Jameroni', 'Pinto', '1960-07-15', '0000-00-00')
       (1, 'Abidi', 'James', '1976-03-05', '0000-00-00');

/* REFERENTIAL INTEGRITY CONSTRAINTS */
/* These violate all the referential integrity constraints because the id's
 which are used as foreign key values do not exist in the parent table */
INSERT INTO MovieGenre
VALUES (9090909090, 'Comedy');

INSERT INTO MovieDirector
VALUES (0909090909, 09090909090);

INSERT INTO MovieActor
VALUES (0909090909, 090909090, "James Bond");

INSERT INTO Review
VALUES ('Asdf', '1960-07-15 23:59:59', 9090909090909, 10, "Hello WORLD!!");

/* CHECK CONSTRAINTS */
/* All of these have negative ID Values which violate the check constraints */
INSERT INTO Movie
VALUES (-1, 'asdf', 'asdff', 'R', 'asdf co');

INSERT INTO Actor
VALUES (-1, 'asdf', 'asdfg', 'Female', '1960-07-15', '0000-00-00');

INSERT INTO Director
VALUES (-1, 'asdf', 'asdfg', 'Female', '1960-08-15', '0000-00-00');

INSERT INTO MovieGenre
VALUES (-1, 'Comedy');

INSERT INTO MovieDirector
VALUES (-1, -1);

INSERT INTO MovieActor
VALUES (-1, -1, "James Bond");

INSERT INTO Review
VALUES ('asdf', '1960-07-15 23:59:59', '-1', '5', 'Hello World');

/* OTHER CONSTRAINTS */
/* All of these have NULL values in the tuple which violates the
 NOT NULL Condition */
INSERT INTO Actor
VALUES (NULL, NULL, NULL, NULL, NULL, NULL);

INSERT INTO Movie
VALUES (NULL, NULL, NULL, NULL, NULL);

INSERT INTO Directors
VALUES (NULL, NULL, NULL, NULL, NULL);
