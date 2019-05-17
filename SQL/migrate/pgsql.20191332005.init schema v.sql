-- Initialize Schema Version Tracking for PostgreSQL

CREATE SCHEMA config;

CREATE TABLE config.groups (
	gid SERIAL PRIMARY KEY,
	name CHAR[32],
	parent INTEGER FOREIGN(config.groups.gid) NULL,
	UNIQUE(name, parent)
);

CREATE TABLE config.data (
	gid SERIAL PRIMARY KEY,
	name CHAR[32],
	ingroup INTEGER FOREIGN(config.groups.gid) NULL,
	val TEXT,		-- Values are limited to TEXT encoding for versitility
	UNIQUE(name, ingroup)
)

-- Entered automaticaly by migration tool on completion:
-- INSERT INTO config.data VALUES( 20191332005 );

