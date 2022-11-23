-- AUfgabe 0
ALTER TABLE
    mahnung
ADD
    vorgaenger Integer;

ALTER TABLE
    mahnung
ADD
    FOREIGN KEY (vorgaenger) REFERENCES mahnung(mahnungoid) ON UPDATE CASCADE;

CREATE TRIGGER TRIGGER_MAHNUNG_VORGAENGER_SET FOR Mahnung
AFTER INSERT
AS
BEGIN
    if(new.Vorgaenger is NULL) then
        UPDATE Mahnung M
        SET M.Vorgaenger = Coalesce(
                                (SELECT FIRST 1 SKIP 1 MahnungOID
                                FROM Mahnung as M
                                WHERE
                                    M.LeserOID = new.LeserOID
                                    AND M.BuchOID = new.BuchOID
                                    AND M.ExID = new.ExID
                                ORDER BY M.Datum DESC),
                                new.mahnungoid
                            )
        WHERE M.MahnungOID = new.MahnungOID;
END

-- Aufgabe 1
SELECT
    B.titel,
    B.verfasser
FROM
    Buch AS B
WHERE
    B.buchoid NOT IN (
        SELECT
            A.buchoid
        FROM
            ausleihe as A
    );

-- Aufgabe 2
SELECT
    DISTINCT (
        SELECT
            B.titel
        FROM
            Buch AS B
        WHERE
            B.buchoid = V.buchoid
    )
FROM
    Vormerkung AS V
WHERE
    V.buchoid IN (
        SELECT
            A.buchoid
        FROM
            Ausleihe AS A
        WHERE
            DATEDIFF(day, V.vormerkdatum, A.spaetestesrdat) > 14
            AND A.rdat IS NULL
    );

-- Aufgabe 3
-- Ich glaube ich schaffe das keine 6 wochen mehr :(
SELECT
    B.Titel B.Verfasser
FROM
    Buch as B
WHERE
    B.BuchOID IN (
        SELECT
            BuchOID
        FROM
            Exemplar as E
        WHERE
            E.anschaffungsdatum = (
                SELECT
                    FIRST 1 min(E2.anschaffungsdatum)
                FROM
                    Exemplar as E2
                GROUP BY
                    E2.BuchOID,
                    E2.ExID,
                    E2.anschaffungsdatum
                ORDER BY
                    E2.anschaffungsdatum ASC
            )
    );

-- Aufgabe 4
SELECT
    b.titel
FROM
    buch as b
WHERE
    b.buchoid = ANY (
        SELECT
            m.buchoid
        FROM
            mahnung as m
        WHERE
            m.datum > '15.10.2012'
    );

-- Aufgabe 5
SELECT
    b.titel
FROM
    buch as b
WHERE
    b.buchoid in (
        SELECT
            DISTINCT e.buchoid
        FROM
            exemplar as e
        WHERE
            EXISTS (
                SELECT
                    v.vormerkdatum
                FROM
                    vormerkung as v
                WHERE
                    v.buchoid = e.buchoid
                    AND v.vormerkdatum < e.anschaffungsdatum
            )
    );

-- Auzgabe 6
SELECT
    L.nachname,
    (
        SELECT
            Count(A.leseroid)
        FROM
            ausleihe as a
        WHERE
            L.leseroid = A.leseroid
    )
FROM
    leser as L;

-- Aufgabe 7
SELECT
    (
        SELECT
            B.titel
        FROM
            buch as b
        WHERE
            b.buchoid = e.buchoid
    ) as titel,
    e.exid
FROM
    exemplar as e
WHERE
    NOT EXISTS (
        SELECT
            a.buchoid,
            a.exid
        FROM
            ausleihe as a
        WHERE
            a.buchoid = e.buchoid
            and a.exid = e.exid
    )