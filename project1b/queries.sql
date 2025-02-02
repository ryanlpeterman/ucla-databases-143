USE CS143;

/* All Actors in movie "Die Another Day" */
SELECT CONCAT(first ," ", last) AS Name
FROM Movie m, MovieActor ma, Actor a
WHERE m.id=ma.mid AND m.title="Die Another Day" AND ma.aid=a.id;

/* Counts number of actors in more than one movie */
SELECT COUNT(*) FROM (SELECT DISTINCT ma1.aid
FROM MovieActor ma1, MovieActor ma2
WHERE ma1.mid!=ma2.mid AND ma1.aid=ma2.aid) AS MULT_MOVE_ACTORS;

/* Actor who has been in the most 'R' rated movies */
SELECT A.first First, A.last Last, COUNT(*) Num_Movies
FROM Movie M, MovieActor Ma, Actor A 
WHERE M.rating='R' AND Ma.aid=A.id AND M.id=Ma.mid
GROUP BY Ma.aid
ORDER BY COUNT(*) DESC
LIMIT 1;

