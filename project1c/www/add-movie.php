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
                        <input type="checkbox" value="Action">Action</input>
                        <input type="checkbox" value="Adult">Adult</input>
                        <input type="checkbox" value="Adventure">Adventure</input>
                        <input type="checkbox" value="Animation">Animation</input>
                        <input type="checkbox" value="Comedy">Comedy</input>
                        <input type="checkbox" value="Crime">Crime</input>
                        <input type="checkbox" value="Documentary">Documentary</input>
                        <input type="checkbox" value="Drama">Drama</input>
                        <input type="checkbox" value="Family">Family</input>
                        <input type="checkbox" value="Fantasy">Fantasy</input>
                        <input type="checkbox" value="Horror">Horror</input>
                        <input type="checkbox" value="Musical">Musical</input>
                        <input type="checkbox" value="Mystery">Mystery</input>
                        <input type="checkbox" value="Romance">Romance</input>
                        <input type="checkbox" value="Sci-fi">Sci-Fi</input>
                        <input type="checkbox" value="Short">Short</input>
                        <input type="checkbox" value="Thriller">Thriller</input>
                        <input type="checkbox" value="War">War</input>
                        <input type="checkbox" value="Western">Western</input>
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

		        //get max_id
			//perform query
			//if fails say why
			//if not increment

			//go through genre checkboxes; if checked, add that movie/genre info into genre table

		      ?>
		    </div>

                </div>
            </div>
        </body>
</html>
