create table ARTIKEL
(
    BEZEICHNUNG      VARCHAR(50),
    NETTOEINZELPREIS DECIMAL(10, 2),
    ID               INTEGER not null
        constraint PK_ARTIKEL
            primary key
);

create table KUNDE
(
    NACHNAME       VARCHAR(100),
    ERSTBESTELLUNG TIMESTAMP WITH TIME ZONE(30),
    ID             INTEGER not null
        constraint PK_KUNDE
            primary key
);

create table BESTELLUNG
(
    ID         INTEGER not null
        constraint FK_BESTELLUNG_0
            references KUNDE,
    BESTELL_ID INTEGER not null,
    OWNER      INTEGER,
    MENGE      INTEGER,
    ARTIKEL    INTEGER
        constraint FK_BESTELLUNG_1
            unique
        constraint FK_BESTELLUNG_1
            references ARTIKEL,
    PREIS      DECIMAL(10, 2)
);

create unique index FK_BESTELLUNG_0
    on BESTELLUNG (ID);

create unique index PK_BESTELLUNG
    on BESTELLUNG (ID);

alter table BESTELLUNG
    add constraint PK_BESTELLUNG
        primary key (ID);

create table KONTOVERBINDUNG
(
    BANKLEITZAHL VARCHAR(30),
    KONTONUMMER  VARCHAR(30),
    ID           INTEGER not null
        constraint FK_KONTOVERBINDUNG_0
            references KUNDE,
    OWNER        INTEGER
);

create unique index FK_KONTOVERBINDUNG_0
    on KONTOVERBINDUNG (ID);

create unique index PK_KONTOVERBINDUNG
    on KONTOVERBINDUNG (ID);

alter table KONTOVERBINDUNG
    add constraint PK_KONTOVERBINDUNG
        primary key (ID);
