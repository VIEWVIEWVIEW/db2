SELECT B.Titel as Titel, Count(B.Titel) as Anzahl
FROM Buch as B
    inner join Ausleihe as A
        on B.BuchOID = A.BuchOID
WHERE (A.RDat > A.spaetestesrdat) OR (A.spaetestesrdat < CURRENT_DATE AND A.RDat is NULL)
GROUP BY B.Titel
ORDER BY Anzahl DESC
