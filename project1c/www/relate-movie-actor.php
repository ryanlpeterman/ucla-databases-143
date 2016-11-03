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
                    <h1> Add Movie Actor Relation </h1>
                    <form method="GET">
                        <h3> Movie Title </h3>
                        <select name="movie">
                        </select>

                        <h3> Actor </h3>
                        <select name="actor">
                        </select>
                        
                        <h3> Title </h3>
                        <input type="text" placeholder="role in movie" name="role"/>            
                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   
                    </form>
                </div>
            </div>
        </body>
</html>
