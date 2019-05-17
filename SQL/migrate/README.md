### Migration Formating Notes ###
Database Migration Files are named in three parts.  
First is the target database engine which is a forward-compatibility feature allowing us the luxury of targeting a variety of database engines from our prefered PostgreSQL which is very feature rich to the more limited SQLite whose saving grace is using local files as databases.  
In the odd case that the code is not database specific, it is recomended that the code is symlinked under a similar name for reuse.  
The second part is the Schema Version, a date-time stamp starting with the four-digit year, three-digit day of the year, the two-digit hour on a 24-hr clock, and the minutes past the hour in the UTC timezone.  
It is not expected that more then one migration shall exist per day in any database much less more then one in a minute, and if this ever where to happen it is intended that there is a merge confict for manual resolution and expected that the comunication planning error that caused it be resolved.  
The third part is a note to the maintainers breifly stating the purpose of the Migration, a point that can further be elaborated in the NEWS file.  
Finaly, migrations should have an appropriate extention such as `.sql` to ensure that human and software editors know what they are looking at.  
However, the migration program only looks at the Engine ID and the Schema Version to do it's work leaving the rest of the name for the benifit of the developers and maintainers.

> &lt;Engine ID&rt;.&lt;Schema Version&rt;.&lt;&amp;c.&rt;

NOTE: The migrator does not actualy care if the schema version is a date-time stamp or not, it only cares about the integer sort sequence extracted from `/[0-9]+/`.  
However, it is considered best practice to use date-time stamps as rolling version codes, and also for numberical file names to be of uniform length to maintain their listing order, and it should go without saying that the very purpose of versioning is to keep things in order, therefore it is imperitive to keep a constant formatting in the versions such as the one above described.

#### Engine IDs ####
Unlike the schema version which is quite flexable, Engine IDs are somewhat more rigid given the requirement for application support.  
The only flexebility available with IDs is that they are not case-sensitive, but the recomendation remains that are all lower case or at very least consistently cased for the sake of future developers and maintainers viewing the list.

> +=============+===============================+
> |  Engine ID  |  Target                       |
> +-------------+-------------------------------+
> |  pgsql      |  PostgreSQL                   |
> |  mysql      |  MySQL                        |
> |  sqlite     |  SQLite                       |
> |  sqlsrv     |  Microsoft(R) SQL Server(TM)  |
> +-------------+-------------------------------+

#### File Creation (Note) ####
For those running the Bash shell, the command `touch "<Engine ID>.$(TZ="GST-00" date "+%Y%j%H%m").<&c.>"` replacing `<Engine ID>` and `<&c.>` as appropriate should create a file of the correct name to work with.

