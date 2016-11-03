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
                </div>
            </div>
        </body>
</html>
