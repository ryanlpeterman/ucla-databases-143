USE CS143;

LOAD DATA LOCAL INFILE '~/data/movie.del'
	INTO TABLE Movie
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/actor1.del'
	INTO TABLE Actor
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/actor2.del'
	INTO TABLE Actor
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/actor3.del'
	INTO TABLE Actor
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/director.del'
	INTO TABLE Director
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/movieactor1.del'
	INTO TABLE MovieActor
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/movieactor2.del'
	INTO TABLE MovieActor
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/moviedirector.del'
	INTO TABLE MovieDirector
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

LOAD DATA LOCAL INFILE '~/data/moviegenre.del'
	INTO TABLE MovieGenre
	FIELDS TERMINATED BY ','
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n';

INSERT INTO MaxPersonID
VALUES (69000);

INSERT INTO MaxMovieID
VALUES (4750);
