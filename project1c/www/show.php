<!DOCTYPE HTML>
<html>
        <head>
            <link type="text/css" rel="stylesheet" href="style/style.css">
            <link href='http://fonts.googleapis.com/css?family=Lato&subset=latin,latin-ext' rel='stylesheet' type='text/css'>
            <title>Project 1C: Movie Website</title>
        </head>
        <body>
            <div id="navicol">
                <a href="index.php"><div id="logo">Project 1C</div></a>
                <a href="add-actor-director.php"><div class="navibutton">Add Actor/Director</div></a>
                <a href="add-movie.php"><div class="navibutton">Add Movie Info</div></a>
		<a href="add-review.php"><div class="navibutton">Add Review</div></a>
                <a href="relate-movie-actor.php"><div class="navibutton">Relate Movie/Actor</div></a>
                <a href="relate-movie-director.php"><div class="navibutton">Relate Movie/Director</div></a>
                <a href="search.php"><div class="navibutton">Search Actors/Movies</div></a>
            </div>

<?php
            $servername = "localhost";
            $username = "cs143";
            $password = "";
            $dbname = "CS143";
            
            // Create connection
            $conn = new mysqli($servername, $username, $password, $dbname);
            
            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            } 

        ?>


            <div id="content">
                <div id="raised-interface">
                    <h1> Display Page </h1>
		    
		    <?php

		       $forms = $_GET;
		       
		       if (... aid?) { // if actor
		         
		         $actor_query = "SELECT * FROM Actor WHERE id=" . $forms["aid"] . ";";
		         $actor_res = $conn->query($actor_query);

		         $row = $actor_res->fetch_assoc();
		    
		         echo "<h1> " . $row["first"] . " " $row["last"] . " </h1>";

		         $movie_query = "SELECT * FROM Movie WHERE mid in (SELECT mid FROM MovieActor WHERE " // finish this

		       } else { // if movie

		       }
		    ?>


                <div id="response">

		</div>
                <br class="clear"/>
                </div>
            </div>


        </body>
</html>
