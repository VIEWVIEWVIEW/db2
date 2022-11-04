SELECT B.Titel as Buchtitel, E.Anschaffungsdatum as angeschafft_am, Count(E.ExID) as Anzahl
FROM Buch as B
    inner join Exemplar as E
        on B.BuchOID = E.BuchOID
GROUP BY B.Titel, E.Anschaffungsdatum;
