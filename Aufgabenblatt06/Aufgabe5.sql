SELECT B.Titel as Titel, E.ExID as ExemplarID, COALESCE(A.spaetestesrdat, 'Exemplar direkt verfuegbar') as Rueckgabedatum
FROM LESER as L
    inner join Vormerkung as V
        on L.LeserOID = V.LeserOID
    inner join Buch as B
        on V.BuchOID = B.BuchOID
    inner join Exemplar as E
        on B.BuchOID = E.BuchOID
    left outer join Ausleihe as A
        on E.BuchOID = A.BuchOID AND E.ExID = A.ExID
WHERE L.Nachname = 'Richts' AND UPPER(L.Geschlecht) = 'W'
    AND B.Titel = 'Grundlagen von Datenbanksystemen' AND B.Verfasser = 'Elmasri'
ORDER BY Rueckgabedatum ASC