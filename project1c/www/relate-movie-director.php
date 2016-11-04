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
                <a href="relate-movie-actor.php"><div class="navibutton">Relate Movie/Actor</div></a>
                <a href="relate-movie-director.php"><div class="navibutton">Relate Movie/Director</div></a>
                <a href="find-actor.php"><div class="navibutton">Find Actor</div></a>
                <a href="find-movie.php"><div class="navibutton">Find Movie</div></a>
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

	       $movie_query = "SELECT title, year, id FROM Movie;";
	       $director_query = "SELECT first, last, dob, id FROM Director";

	       $movie_res = $conn->query($movie_query);
	       $director_res = $conn->query($director_query);
	    ?>


            <div id="content">
                <div id="raised-interface">
                    <h1> Add Movie Director Relation </h1>
                    <form method="GET">
                        <h3> Movie Title </h3>
                        <select name="movie">
			  <?php
			     while($row = $movie_res->fetch_assoc()) {
			       echo "<option value='" . $row["id"] . "'> " . $row["title"] . " (" . $row["year"] . ")</option>";
			     }
			  ?>
                        </select>

                        <h3> Director </h3>
                        <select name="director">
			  <?php
			     while($row = $director_res->fetch_assoc()) {
			       echo "<option value='" . $row["id"] . "'> " . $row["first"] . " " . $row["last"] . " (" . $row["dob"] . ")</option>";
			     }
			  ?>
                        </select>
                        
                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   
                    </form>

		<div id="response">
		  <?php
		     $correct_formatting = true;
		     $forms = $_GET;
		     if (empty($forms)) {
		       $correct_formatting = false;
		     }

		     foreach($forms as $key => $value) {
		       if ($value == "") {
		         echo $key . " must be specified!<br>";
		         $correct_formatting = false;
		       }
		     }

		     if ($correct_formatting) {
		       $query = "INSERT INTO MovieDirector VALUES (" . $forms["movie"] . ", " . $forms["director"] . ");";

		       $result = $conn->query($query);
		  
		       if (!$result) {
		         echo "Invalid Query!";
		       } else {
		         echo "Added Successfully!";
		       }

		     }

		     $conn->close();

		  ?>
		    </div>
               <br class="clear"/>
                </div>
            </div>
        </body>
</html>
