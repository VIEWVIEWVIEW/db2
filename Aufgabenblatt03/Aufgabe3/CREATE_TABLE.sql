CREATE TABLE Vornamen
(
    VornameOID int,
    Vorname varchar(35),
    constraint PK_Vornamen primary key(VornameOID)
);
CREATE TABLE Nachnamen
(
    NachnameOID int,
    Nachname varchar(55),
    constraint PK_Nachnamen primary key(NachnameOID)
);
CREATE TABLE Orte
(
    OrtOID int,
    Ort varchar(55),
    constraint PK_Orte primary key(OrtOID)
);
CREATE TABLE Strassen
(
    StrOID int,
    Strasse varchar(55),
    constraint PK_Strassen primary key(StrOID)
);

-- Create autoincrements

CREATE SEQUENCE SEQ_VORNAMEN;
ALTER SEQUENCE SEQ_VORNAMEN RESTART WITH 0;

CREATE TRIGGER VORNAMEN_ID FOR VORNAMEN
BEFORE INSERT AS BEGIN
	if(new.VornameOID is NULL) THEN
		new.VornameOID = NEXT VALUE FOR SEQ_VORNAMEN;
END

-- Nachnamen
CREATE SEQUENCE SEQ_NACHNAMEN;
ALTER SEQUENCE SEQ_NACHNAMEN RESTART WITH 0;

CREATE TRIGGER NACHNAMEN_ID FOR NACHNAMEN
BEFORE INSERT AS BEGIN
	if(new.NachnameOID is NULL) THEN
		new.NachnameOID = NEXT VALUE FOR SEQ_NACHNAMEN;
END

-- Strassen
CREATE SEQUENCE SEQ_STRASSEN_ID;
ALTER SEQUENCE SEQ_STRASSEN_ID RESTART WITH 0;

CREATE TRIGGER STRASSEN_ID for STRASSEN
BEFORE INSERT AS
BEGIN
    if(new.StrOID is NULL) THEN
        new.StrOID = NEXT VALUE FOR SEQ_STRASSEN_ID;
END


-- Orte
CREATE SEQUENCE SEQ_ORTE_ID;
ALTER SEQUENCE SEQ_ORTE_ID RESTART WITH 0;

CREATE TRIGGER ORTE_ID for ORTE
BEFORE INSERT AS
BEGIN
    if(new.OrtOID is NULL) THEN
    BEGIN
        new.OrtOID = gen_id(seq_orte_id,2); -- Der Nummernkey der Tabelle Ort soll Keywerte jeweils in Zweierschritten erhalten
    END
END


-- Test Inserts
INSERT INTO Vornamen (VornameOID,Vorname) VALUES (NULL,'Marc');
INSERT INTO Vornamen (VornameOID,Vorname) VALUES (NULL,'Alexander');
INSERT INTO Vornamen (VornameOID,Vorname) VALUES (NULL,'Mark');
