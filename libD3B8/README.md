### libD3B8 (lib-debate, not lib-54,200) ###
The name is a cleaver play on the common venacular of function inputs being  
called "arguments"; when one argues it's a debate, in 1337 "e" becomes "3", and  
common IM shorthand reduces "-ate" to "-8" resulting in "D3B8" which is a valid  
Hexadecimal code which is funny because we are programmers with no [other] life  
XD.

Now I've explained the joke, libD3B8 is an argument processing library [^^]  
taking the normal `int argc` and `char **argv` values and processing them   
against a callback system (the simplest and most versital way we could think of  
fullfilling it's function).  There are two kinds of arguments by default:  
shorthand and full-length arguments.  

#### Why An[other] Argument Processing Library? ####
It has been said that good programmers are lazy programmers, programmers that  
do not reinvent the wheel and reuse code where possable.  However, a good  
engineer always knows that the time to reinvent the wheel is when the existing  
wheel does not do the job as intended.  Therefore, as a good architect, we have  
designed this utility to fill our need to process program arguments in the  
simplest way possable.  And as a result, applications using this library:  
1. Are easier to read for using simple library configuration commands instead   
of complex parsing logic,
2. Have [relatively] complete control over argument processing and structure  
(the only imagned scenario this library does not support is the case of  
random access arguments, anything else is estimated possable via cleaver abuse  
of library features)
3. Do not have to concern themselves with what's really hapening undreneath;  
the library abstracts argument interpreitation to Keep It Simple and Sincere,  
a truth that all can see: we are the automation and we won't think for you

#### Initializing D3B8cfg Object ####
In conformance with best practices, libD3B8 does not use a global configuration  
although it can be simulated by an application by using a global D3B8cfg object.  
Doing this is considered best practice not only because it does not use global  
variables (considered a poor design choice in general, and especialy for  
libraries where global variables may clash with any other global variables  
defined by the application or other libraries) but also because using local  
objects enables the user to create far more advanced configurations when they  
need to reguarding the use of multiple and dynamic configurations.

When creating a D3B8cfg object it is important to remember that definition and  
initialization do not occure simultaniously (see _Duplicating D3B8cfg Objects_),  
therefore the common form of initialization is:
> D3B8cfg myconf;
> D3B8\_init( myconf );

And decomissionaing a D3B8cfg object is even simpler:
> D3B8\_free( myconf );

But we must warn never to attempt assignment, e.g.
> D3B8cfg myconf\_bkup, myconf\_active;
> myconf\_bkup = myconf\_active;
which will result in a kind of shallow copy when an deep-copy was intended.  
Instead use the routines described in __Duplicaing D3B8cfg Objects__ such  
assignment operations.

#### Shorthand Arguemnts ####
Shorthand arguments consist of single-letter codes that may be chained together  
in a single argument; the default prefix for Shorthand arguments is `-` such  
that any argument staring with `-` will begin to be processed as shorthand.  
However, if the shorthand prefix is a substring of the full-length prefix, the  
full-length prefix will be checked for before the shorthand; and in the event  
that the full-length prefix and shorthand prefix is the same, arguments will be  
checked as a full-length option first and then only as shorthand if there are  
no matches in the full-length option list.  

To add a shorthand trigger, the user will call
> D3B8\_addshort( D3B8cfg &amp;, char, ushort, (bool)(\*cb)(int, char \*\*) );
where `char` is the shorthand identifier, `ushort` is the number of arguments  
to pop off the stack for the callback, and `cb` is the function call that will  
be made on detection.  The callback receives the string array `char **` and  
it's length `int` for further processing where `char *[0]` is the argument  
being processes `char *[1]` is the shorthand value that triggered the callback.  
The reurn value is a sucess (true) or failure (false) of processing; it is up  
to the callback to handle the failure before returning the value, but by  
returning failure the engine knows to report it as unprocessed.

To add a shorthand prefix, the user will call
> D3B8\_addshort\_prefix( D3B8cfg &amp;, const char \* )
and to remove a shorthand prefix, the user will call 
> D3B8\_delshort\_prefix( D3B8cfg &amp;, const char \* )
where `char *` is a NULL-terminated C-string.[1][Non-C Programers Note: strings  
encapsulated in double-quotes are automaticaly NULL-Terminated while strings  
encapsulated in single-quotes are not, e.g. "IAM" == 'IAM\\0']

NOTE: The ability to delete prefixes exists only because there is a default mode  
that the user may want to override; it is not expected that the need to delete  
triggers should ever arise as it is expeted that the user configure them the way  
they want it up front without any need to change it.  Any oversight in this line  
of thought is accounted for in the _Duplicating D3B8cfg Objects_ section.

#### Full-length Arguements ####
Full-length arguments are more versital then Shorthand in that they use RegEx  
for defining matches and return the submatches in order as arguments.  Because  
using RegEx can enable false-positives (not something we can fix without  
implimenting our own RegEx Engine [TODO]) full-length arguments use priorities  
to tune the result.[2][Priorities will go away when we use a custom RegEx  
engine]  

To add a full-length trigger, the user will call
> D3B8\_add( D3B8cfg &amp;, const char \*RegEx, ushort, (bool)(\*cb)(int, char \*\*), short priority = 0 )
which is identical to the shorthand call except that `RegEx` is a  
NULL-terminated C-string[1][] containing a Regular Expression, and `priority`  
is a modifire to manualy correct the order that Regular Expressions are checked  
in; there are no guarentees on the order in which RegEx will be checked in at  
the same level of prioritization (and there never will be).

Users will note that the number of C-Strings passed to the callback will  
include the RegEx submatches in order of occurence starting at `char *[1]`.  
The submatches are not differentiated from other arguemtns for the callback as  
the callback is expected to know about what the RegEx is returning.

To add a full-length prefix, the user will call
> D3B8\_add\_prefix( D3B8cfg &amp; const char \* )
and to remove a full-length prefix, the user will call
> D3B8\_del\_prefix( D3B8cfg &amp;, const char \* )
where `char *` is a NULL-terminated C-String.[1][]  
By default the full-length prefix is `--` as is common.

#### Unprocessed Arguments ####
Arguments that are not used by Full-length or Shorthand filters will be passed  
first to a subrutine of the user's choosing for last-chance processing, and  
the subrutine will return either success (true) or failure (false) to determine  
if the argument should be left in place or purged from the argument list; if no  
subrutine is defined (default) then all unmatched/unprocessed arguements will  
be left in place for the user to deal with at their disgression.

To set the subrutine the user will call
> D3B8\_set\_unproc( D3B8cfg &amp;, (bool)(\*cb)(int, char\*\*))
where the number of arguments passed will always be 1 for full-length arguments  
and contain the unprocessed argument and 2 for Shorthand arguments conting the  
unprocessess argument first and the unprocessed character second, all  
NULL-terminated C-Strings[1][].

To clear the subroutine we provide the macro
> \#define D3B8\_clear\_unproc( cfg ) D3B8\_set\_unproc( cfg, NULL );
for your conviniance.

##### Purging Arguments #####
Purging arguments happens automaticly as the engine is done with them, a  
feature easily controlled using 
> D3B8\_set\_purgemode( D3B8cfg &amp;, const short &amp;mode )

There are three purge modes supported:
> +======+=====================================================+
> | Mode | Description                                         |
> +------+-----------------------------------------------------+
> |   1  | Purges Arguments when the callback returns success  |
> |   0  | Disables the purge feature, no arguments are purged |
> |  -1  | Purges Aruments when all callbacks return failure   |
> +------+-----------------------------------------------------+

The default purge mode is `1` considering that it is most likely that the user  
wants to attempt reporting or correction of the unprocessed arguments with a  
second pass.  Alternativly, purging can be disabled using the `0` mode allowing  
the application to maintain the arguments as received for whatever reason while  
costing us nothing.  And the `-1` mode is the oposite of the `1` mode in the  
off-chance that the user may want to use a corrected verion of the received  
arguments at some future point in execution (save them to file maybe?).

For conviniance the following macros are defined for the purge mode:
> \#define D3B8\_PURGEMODE\_ONSUCCESS 1
> \#define D3B8\_PURGEMODE\_ONFAILURE -1
> \#define D3B8\_PURGEMODE\_DISABLE 0
> \#define D3B8\_PURGEMODE\_DEFAULT D3B8\_PURGEMODE\_ONSUCCESS
> \#define D3B8\_set\_purge\_onsuccess( cfg ) D3B8\_set\_purgemode( cfg, D3B8\_PURGEMODE\_ONSUCCESS );
> \#define D3B8\_set\_purge\_onfailure( cfg ) D3B8\_set\_purgemode( cfg, D3B8\_PURGEMODE\_ONFAILURE );
> \#define D3B8\_set\_purge\_disable( cfg ) D3B8\_set\_purgemode( cfg, D3B8\_PURGEMODE\_DISABLE );
> \#define D3B8\_set\_purge\_default( cfg ) D3B8\_set\_purgemode( cfg, D3B8\_PURGEMODE\_DEFAULT );

#### Duplicating D3B8cfg Objects ####
Due to the nature of D3B8cfg objects, there are three copy modes that we  
consider to mention; first is the shallow copy which is essentialy perfomed  
duplicating it's reference to a new variable such that we are actualy the  
same object proverbialy rebranded [3][Shallow copies of D3B8 objects are not  
reccomended except in expert situation and you will get a warning from the  
preprocessor if `D3B8_EXPERT` is not defined.].  The second copy mode is an  
inheritance mode such that the copied object is actualy an empty D3B8cfg object  
with an inheritance reference to the original D3B8cfg object.  It is generaly  
recommended to use the third copy mode which is a deep-copy of the D3B8cfg  
object where the new object is conceptualy a fork of the original.

It is recomended that the destination object is not initialized at the time of  
copy in order to reduce overhead in reinitializing the object; when any D3B8cfg  
object is copied, the new location is checked to see if it has been initialized  
and if it has then it is decomissioned and reinitilaized by the copy routine in  
order to ensure that the destination is ready for the operation.

The user is provided the following macros/routines:[4][These routines are  
wrapper macros that we will leave to the source code to fully define.]
> D3B8\_copycfg\_shallow( D3B8cfg &amp;orig, D3B8cfg &amp;dest )
> D3B8\_copycfg\_decend( D3B8cfg &amp;orig, D3B8cfg &amp;dest )
> D3B8\_copycfg\_deep( D3B8cfg &amp;orig, D3B8cfg &amp;dest )

##### Inheriting a D3B8cfg Object #####
Configuration inheritance is something of a complicated issue, we resolve it by  
recursion: 
* if the invoked D3B8cfg object is unable to process an object, it tries to  
process it with the D3B8cfg objects it inherits from before declaring it  
unprocessed,
* and if it must be declared unprocessed and the user has not defined an unproc  
callback for the current D3B8cfg object it will check the D3B8cfg objects it  
inherits from for a user defined routine before defaulting.

Inheritance it a powerful tool for such as a plug in system where the loader  
may take arguments and pass whatever it can't process to it's modules; insuch a  
case inheritance would help automate the recursion instead of requiring the  
user to manualy call multiple processing routines which could prove difficult  
to correctly sychronize.

To manualy interact with the inheritance system, the user will call
> D3B8\_add\_inheritance( D3B8cfg &amp;base, D3B8cfg &amp; target, short priority = 0);
> D3B8\_del\_inheritance( D3B8cfg &amp;base, D3B8cfg &amp; target );
where `target` is the D3B8cfg object being inherited by `base` and `priority`  
is a tuning tool used target certain inheritances before others (higher  
priorities come first).


#### Deleting Triggers ####
There are very few cases where the need to delete triggers arises, in most  
cases we expect to create a configuration, use it to perfom argument processing,  
and either destroy it at the beginning of the program to free up memory for  
execution or just forget about it as many programers are inclined to do.  
However we provide two ways to delete triggers for the sake of completeness:

First, and the most likely to be used, is the delete by callback reference  
routine.  Any trigger using the specified callback will be deleted:
> D3B8\_del\_trigger( D3B8cfg &amp;, (bool)(\*cb)(int, char\*\*) )
This is expected to be required in modular applications where by unloading a  
library the calback will cease to be valid.

Second is the delete by rule routines; in full-length triggers the rule is  
identified by the RegEx while in Shorthand it is identified by the character:
> D3B8\_delshort( D3B8cfg &amp;, char );
> D3B8\_del( D3B8cfg &amp;, const char * );
These are provided for completeness, and if you're using them then you're doing  
strange and beautiful things.

