-- Snapshots definition
--DROP TABLE Snapshots;

CREATE TABLE Snapshots (
	"key" INTEGER NOT NULL,
	title TEXT NOT NULL,
	descr TEXT,
	lat REAL NOT NULL,
	lon REAL NOT NULL,
	alt INTEGER,
	CONSTRAINT SNAPSHOTS_PK PRIMARY KEY ("key")
);

CREATE UNIQUE INDEX idx_Snapshots_lat_lon ON Snapshots (lat,lon);
CREATE UNIQUE INDEX idx_Snapshots_title ON Snapshots (title);