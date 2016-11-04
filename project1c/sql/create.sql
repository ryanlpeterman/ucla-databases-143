USE CS143;

/* Drop order matters due to foreign key constraints */
DROP TABLE IF EXISTS MovieGenre; 
DROP TABLE IF EXISTS MovieDirector;
DROP TABLE IF EXISTS MovieActor;
DROP TABLE IF EXISTS Review;
DROP TABLE IF EXISTS MaxPersonID;
DROP TABLE IF EXISTS MaxMovieID;

DROP TABLE IF EXISTS Movie;
DROP TABLE IF EXISTS Actor;
DROP TABLE IF EXISTS Director;

CREATE TABLE Movie 
(
	id INT,
	title VARCHAR(100) NOT NULL,
	year INT,
	rating VARCHAR(10),
	company VARCHAR(50),
    PRIMARY KEY(id),
    CHECK (id>0)
) ENGINE=INNODB;

CREATE TABLE Actor
(
	id INT,
	last VARCHAR(20) NOT NULL,
	first VARCHAR(20) NOT NULL,
	sex VARCHAR(6),
	dob DATE,
	dod DATE NOT NULL,
    PRIMARY KEY(id),
    CHECK(id>0)
) ENGINE=INNODB;

CREATE TABLE Director
(
	id INT,
	last VARCHAR(20) NOT NULL,
	first VARCHAR(20) NOT NULL,
	dob DATE,
	dod DATE NOT NULL,
    PRIMARY KEY(id),
    CHECK(id>0)
) ENGINE=INNODB;

CREATE TABLE MovieGenre
(
    mid INT,
    genre VARCHAR(20),
    FOREIGN KEY(mid)
        REFERENCES Movie(id)
        ON DELETE CASCADE,
    CHECK(mid>0)
) ENGINE=INNODB;

CREATE TABLE MovieDirector
(
    mid INT,
    did INT,
    FOREIGN KEY(mid)
        REFERENCES Movie(id)
        ON DELETE CASCADE,
    FOREIGN KEY(did)
        REFERENCES Director(id)
        ON DELETE CASCADE,
    CHECK(mid>0 AND did>0)
) ENGINE=INNODB;

CREATE TABLE MovieActor
(
    mid INT,
    aid INT,
    role VARCHAR(50),
    FOREIGN KEY(mid)
        REFERENCES Movie(id)
        ON DELETE CASCADE,
    FOREIGN KEY(aid)
        REFERENCES Actor(id)
        ON DELETE CASCADE,
    CHECK(mid>0 AND aid>0)
) ENGINE=INNODB;

CREATE TABLE Review
(
    name VARCHAR(20),
    time TIMESTAMP,
    mid INT,
    rating INT,
    comment VARCHAR(500),
    FOREIGN KEY(mid)
        REFERENCES Movie(id)
        ON DELETE CASCADE,
    CHECK(mid>0)
) ENGINE=INNODB;

CREATE TABLE MaxPersonID
(
    id INT
) ENGINE=INNODB;

CREATE TABLE MaxMovieID
(
    id INT
) ENGINE=INNODB;

