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

            <div id="content">
                <div id="raised-interface">
                    <h1> Add New Movie </h1>
                    <form method="GET">
                        <h3> Title </h3>
                        <input type="text" placeholder="title of movie" name="title"/>            
                        <h3> Company </h3>
                        <input type="text" placeholder="company name" name="company"/>            
                        <h3> Year </h3>
                        <input type="text" placeholder="year of make" name="year"/>            
                        
                        <h3> MPAA Rating </h3>
                        <select name="rating">
                            <option value="G">G</option>
                            <option value="PG">PG</option>
                            <option value="NC-17">NC-17</option>
                            <option value="PG-13">PG-13</option>
                            <option value="R">R</option>
                        </select>

                        <h3> Genre </h3>
                        <input type="checkbox" name="genre_list[]" value="Action">Action</input>
                        <input type="checkbox" name="genre_list[]" value="Adult">Adult</input>
                        <input type="checkbox" name="genre_list[]" value="Adventure">Adventure</input>
                        <input type="checkbox" name="genre_list[]" value="Animation">Animation</input>
                        <input type="checkbox" name="genre_list[]" value="Comedy">Comedy</input>
                        <input type="checkbox" name="genre_list[]" value="Crime">Crime</input>
                        <input type="checkbox" name="genre_list[]" value="Documentary">Documentary</input>
                        <input type="checkbox" name="genre_list[]" value="Drama">Drama</input>
                        <input type="checkbox" name="genre_list[]" value="Family">Family</input>
                        <input type="checkbox" name="genre_list[]" value="Fantasy">Fantasy</input>
                        <input type="checkbox" name="genre_list[]" value="Horror">Horror</input>
                        <input type="checkbox" name="genre_list[]" value="Musical">Musical</input>
                        <input type="checkbox" name="genre_list[]" value="Mystery">Mystery</input>
                        <input type="checkbox" name="genre_list[]" value="Romance">Romance</input>
                        <input type="checkbox" name="genre_list[]" value="Sci-fi">Sci-Fi</input>
                        <input type="checkbox" name="genre_list[]" value="Short">Short</input>
                        <input type="checkbox" name="genre_list[]" value="Thriller">Thriller</input>
                        <input type="checkbox" name="genre_list[]" value="War">War</input>
                        <input type="checkbox" name="genre_list[]" value="Western">Western</input>
                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   
                    </form>

		    <div id="response">
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
		         $correct_formatting = true;

		         $forms = $_GET;
		         if (empty($forms)) {
		           $correct_formatting = false;
		         }
		      
		         // check that title and comapny are not empty
		         if ($forms["title"] == "") {
		           echo "Title must be specified!<br>";
		           $correct_formatting = false;
		         }
		         if ($forms["company"] == "") {
		           echo "Company must be specified!<br>";
		           $correct_formatting = false;
		         }

		         // check for valid year
		         $input_year = $forms["year"];
		         $year_len = strlen($input_year);
		         if (year_len != 4) {
		           echo "Year is invalid!<br>";
		           $correct_formatting = false;
		         } else {
		           for ($i = 0; $i < $year_len; $i++) {
			     $char = $input_year[$i];
			     // if any of the 4 chars is not numeric
			     if (!is_numeric($char)) {
			       echo "Year is invalid!<br>";
			       $correct_formatting = false;
                               break;
			     }
			   }
			 }
			 if ($correct_formatting) {
			   if (intval($input_year) < 1000 or intval($input_year) > 9999) {
			     echo "Year is invalid!<br>";
			     $correct_formatting = false;
			   }
			 }
			if ($correct_formatting) {
			  // get the max_id
			  $max_id_res = $conn->query("SELECT * FROM MaxMovieID");
			  $max_id = $max_id_res->fetch_assoc()["id"];

			  $query = "INSERT INTO Movie VALUES (" . $max_id . ", '" . $forms["title"] . "', " . $forms["year"] . ", '" . $forms["rating"] . "', '" . $forms["company"] . "');";
			  $result = $conn->query($query);

			  if (!$result) {
			    echo "Invalid Query!";
			  } else {
			    // if all went well, update the max movie id
			    $update_query = "UPDATE MaxMovieID SET id=id+1;";
			    $conn->query($update_query);

			    //go through and add genre information
			    

			    echo "Added Successfully!";
			  }

			}
					    
		      ?>
		    </div>

                </div>
            </div>
        </body>
</html>
