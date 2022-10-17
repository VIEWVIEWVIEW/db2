CREATE TABLE Kunde(
    Nachname varchar(100),
    Erstbestellung TIMESTAMP WITH TIME ZONE default CURRENT_TIMESTAMP,
    id int NOT NULL
);

CREATE TABLE Kontoverbindung(
    Bankleitzahl varchar(30),
    Kontonummer varchar(30),
    id int NOT NULL,
    Owner int
);

CREATE TABLE Bestellung(
    id int NOT NULL,
    Bestell_id int NOT NULL,
    Owner int,
    Menge int,
    Artikel int,
    Preis decimal(10,2)
);

CREATE TABLE Artikel(
    Bezeichnung varchar(50),
    Nettoeinzelpreis decimal(10,2),
    id int NOT NULL
);

ALTER TABLE Kunde ADD CONSTRAINT PK_Kunde PRIMARY KEY(id);
ALTER TABLE Kontoverbindung ADD CONSTRAINT PK_Kontoverbindung PRIMARY KEY(id);
ALTER TABLE Bestellung ADD CONSTRAINT PK_Bestellung PRIMARY KEY(id);
ALTER TABLE Artikel ADD CONSTRAINT PK_Artikel PRIMARY KEY(id);

ALTER TABLE Kontoverbindung ADD CONSTRAINT FK_Kontoverbindung_0 FOREIGN KEY(id) REFERENCES Kunde(id) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Bestellung ADD CONSTRAINT FK_Bestellung_0 FOREIGN KEY(id) REFERENCES Kunde(id) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE Bestellung ADD CONSTRAINT FK_Bestellung_1 FOREIGN KEY(Artikel) REFERENCES Artikel(id) ON DELETE NO ACTION ON UPDATE NO ACTION;
