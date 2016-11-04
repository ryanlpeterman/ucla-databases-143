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
		        <h1>Display Page</h1> 
		    <?php
                function echo_mysql_res($result) {
                   // holds the header of the table 
                   $column_header = "<table><tr>";
                   // holds fields for easy access later
                   $header_fields = array();
                   
                   foreach (($result->fetch_fields()) as $value) {
                           // for every column we append to html table
                           if ($value->name != "aid" and $value->name != "mid" and $value->name != "id") {
                               $column_header = $column_header . "<th>". $value->name . "</th>";
                               array_push($header_fields, $value->name);
                           }
                   }
                   // close header
                   $column_header = $column_header . "</tr>";
                   echo $column_header;
                   
                   if ($result->num_rows > 0) {
                        // output data of each row
                        while($row = $result->fetch_assoc()) {
                               echo "<tr>";
                                foreach ($header_fields as $header) {
                                    if($header == "last" or $header == "first" or $header == "dob" or $header == "sex" or $header == "dod") {
                                        echo "<td><a href='show.php?aid=" . $row["id"] . "'>" . $row[$header] . "</a></td>";
                                    } else {
                                        echo "<td><a href='show.php?mid=" . $row["id"] . "'>" . $row[$header] . "</a></td>";
                                    }
                                }     
                               echo "</tr>";
                        }
                        echo "</table>";
                   }  
                }
		       $forms = $_GET;
		       
		       if ($forms["aid"]) { // if actor
		         
		         $actor_query = "SELECT * FROM Actor WHERE id=" . $forms["aid"] . ";";
                 echo_mysql_res($conn->query($actor_query)); 
                 
                 echo "<h3> Actor's Movies and Roles </h3>";
                 $movies_query = "SELECT * FROM Movie M, MovieActor MA WHERE MA.aid=" . $forms["aid"] . " AND MA.mid=M.id;";
                 echo_mysql_res($conn->query($movies_query)); 
		       } else { // if movie
                    
                    $movie_query = "SELECT * FROM Movie WHERE id=" . $forms["mid"] . ";";
                    echo_mysql_res($conn->query($movie_query));
                    
                    echo "<h3> Actors in this Movie </h3>";
                    $actors_query = "SELECT * FROM Actor A, MovieActor MA WHERE MA.mid=" . $forms["mid"] . " AND MA.aid=A.id";
                    echo_mysql_res($conn->query($actors_query));
                    
                    echo "<h3> User Review Summary </h3>";
                    $summary_query = "SELECT COUNT(*) AS cnt, AVG(rating) AS avg FROM Review WHERE mid=" . $forms["mid"] .";";                
                    $summary_res = $conn->query($summary_query);
                    $aggregate_row = $summary_res->fetch_assoc();
                    
                    // if there are comments
                    if ($aggregate_row["avg"]) {
                        echo "<p> Average score for this movie is " . $aggregate_row["avg"] . " based on " . $aggregate_row["cnt"] . " review(s) </p>";

                         
                        echo "<h3> Comments </h3>";
                        $comments_query = "SELECT * FROM Review WHERE mid=". $forms["mid"] . ";";
                        $comments_res = $conn->query($comments_query);
                        echo_mysql_res($comments_res);
                    } else {
                        echo "<p> No one has reviewed this movie yet! </p>";
                    }
                    echo "<form action='add-review.php'><input type='submit' value='Add Comment'></input></form>";
		       }
		    ?>


                <div id="response">

		</div>
                <br class="clear"/>
                </div>
            </div>


        </body>
</html>
