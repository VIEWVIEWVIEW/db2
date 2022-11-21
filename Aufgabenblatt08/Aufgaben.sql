-- Aufgabe 1
SELECT
    (
        (
            SELECT
                Cast(Count(M.BuchOID) AS DECIMAL(4, 1))
            FROM
                Mahnung AS M
            WHERE
                M.MahnungOID = M.Vorgaenger
        ) / Cast(Count(A.BuchOID) AS DECIMAL(4, 1)) * 100
    ) AS Prozent
FROM
    Ausleihe AS A;

-- Aufgabe 2
ALTER TABLE
    Exemplar
ADD
    AnschBetrag DECIMAL;

--
SELECT
    B.titel,
    E.exid
FROM
    Exemplar AS E,
    Buch AS B
WHERE
    B.Buchoid = E.buchoid
    AND extract(
        year
        FROM
            E.Anschaffungsdatum
    ) = '2022'
    AND cast(E.AnschBetrag AS DECIMAL(9, 2)) < (
        SELECT
            sum(M.Betrag)
        FROM
            Mahnung AS M
        WHERE
            M.BuchOID = E.BuchOID
            AND M.ExID = E.ExID
        GROUP BY
            M.BuchOID,
            M.ExID
    );

-- test:
-- "Java ist eine Insel", BuchOID = 4, ExID = 1 (es gibt nur 1 exemplar), anschaffungsbetrag = 1.00, Mahnungsgebühren = 4.00
-- Aufgabe 3
SELECT
    (
        SELECT
            B.Titel
        FROM
            Buch AS B
        WHERE
            B.BuchOID = M.BuchOID
    ) AS Titel,
    M.Exid AS Exemplar_ID
FROM
    Mahnung AS M
    INNER JOIN Exemplar AS E ON M.BuchOID = E.BuchOID
    AND M.ExID = E.ExID
WHERE
    Extract(
        year
        FROM
            E.Anschaffungsdatum
    ) = '2022'
GROUP BY
    M.BuchOID,
    M.ExID,
    E.AnschBetrag
HAVING
    E.AnschBetrag < sum(M.Betrag);

-- Aufgabe 4
SELECT
    L.Nachname AS Nachname
FROM
    Leser AS L
WHERE
    L.LeserOID IN (
        SELECT
            M.LeserOID
        FROM
            Mahnung AS M
        WHERE
            EXISTS (
                SELECT
                    V.LeserOID
                FROM
                    Vormerkung AS V
                WHERE
                    Extract(
                        year
                        FROM
                            V.Vormerkdatum
                    ) = '2022'
                    AND M.LeserOID = V.LeserOID
                    AND M.BuchOID = V.BuchOID
            )
    );

-- Aufgabe 5
SELECT
    B.Titel AS Buchtitel,
    E.ExID,
    (
        CASE
            WHEN E.ExID IN (
                SELECT
                    A.ExID
                FROM
                    Ausleihe AS A
                WHERE
                    A.BuchOID = E.BuchOID
                    AND A.Rdat IS NULL
            ) THEN 'ausgeliehen'
            ELSE 'nicht ausgeliehen'
        END
    ) AS STATUS
FROM
    buch AS b,
    exemplar AS e
WHERE
    b.buchoid = e.buchoid
    AND b.titel = 'Die Bibel';

-- Aufgabe 6
SELECT
    B.Titel,
    S.Exid,
    S.Status
FROM
    (
        SELECT
            b.buchoid,
            b.titel,
            e.Exid,
            (
                CASE
                    WHEN E.ExID IN (
                        SELECT
                            A.ExID
                        FROM
                            Ausleihe AS A
                        WHERE
                            A.BuchOID = E.BuchOID
                            AND A.Rdat IS NULL
                    ) THEN 'ausgeliehen'
                    ELSE 'nicht ausgeliehen'
                END
            ) AS STATUS
        FROM
            buch AS b,
            Exemplar AS e
        WHERE
            b.buchoid = e.buchoid
    ) AS S
    INNER JOIN Buch AS B ON (
        b.buchoid = s.buchoid
        AND b.titel = s.titel
    )