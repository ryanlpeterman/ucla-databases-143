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

	      $movie_query = "SELECT title, year, id FROM Movie;";
	      $movie_res = $conn->query($movie_query);

           ?>

            <div id="content">
                <div id="raised-interface">
                    <h1> Add Review </h1>
                    <form method="GET">
                        <h3> Name </h3>
                        <input type="text" placeholder="Anonymous" name="name"/>            

			<h3> Movie Title </h3>
			<select name="movie">
			  <?php
			     while($row = $movie_res->fetch_assoc()) {
			       echo "<option value='" . $row["id"] . "'> " . $row["title"] . " (" . $row["year"] . ")</option>";
			     }
			  ?>
			</select>

                        <h3> Rating </h3>
                        <select name="rating">
                            <option value="1">1</option>
			    <option value="2">2</option>
			    <option value="3">3</option>
			    <option value="4">4</option>
			    <option value="5">5</option>
                        </select>

			<h3> Comment </h3>
			<input type="textarea" placeholder="your comment here" name="comment"/>

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

			 if ($correct_formatting) {
			   $reviewer_name = $forms["name"];
			   if ($reviewer_name == "") {
			     $reviewer_name = "Anonymous";
			   }

			   $query = "INSERT INTO Review VALUES ('" . $reviewer_name . "', NOW(), " . $forms["movie"] . ", " . $forms["rating"] . ", '" . $forms["comment"] . "');";
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
