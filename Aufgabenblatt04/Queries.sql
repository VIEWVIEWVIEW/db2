-- Aufgabe 1
SELECT B.Titel as Titel, E.ExID as ExemplarID, E.anschaffungsdatum
FROM Buch as B
inner join Exemplar as E
    on B.BuchOID = E.BuchOID
	
-- Aufgabe 2
SELECT * FROM Leser as L
inner join Vormerkung as V
on L.LeserOID = V.LeserOID

-- Aufgabe 3
SELECT B.Titel as Titel, E.ExID as ExemplarID
FROM Mahnung as M
inner join Exemplar as E
    on M.BuchOID = E.BuchOID AND M.ExID = E.ExID
inner join Buch as B
    on E.BuchOID = B.BuchOID
inner join Vormerkung as V
    on B.BuchOID = V.BuchOID


-- Aufgabe 4
SELECT B.Titel as Titel, Count(M.ExID) as Anzahl_Mahnungen
FROM Mahnung as M
inner join Buch as B
    on M.BuchOID = B.BuchOID
WHERE B.Titel LIKE 'Java ist eine Insel'
GROUP BY B.Titel

-- Aufgabe 5
SELECT DISTINCT B.Titel as Titel, L.Nachname
FROM Leser as L
inner join Mahnung as M
    on L.LeserOID = M.LeserOID
inner join Buch as B
    on M.BuchOID = B.BuchOID
WHERE B.Titel LIKE 'Grundlagen von Datenbanksystemen'

-- Aufgabe 6
Select * From Exemplar as E, mahnung as M
Where E.anschaffungsdatum > M.Datum
 And E.BuchOID = M.BuchOID
 And E.ExID = M.ExID

-- Hier wird zuerst das cross product mit allen möglichen Untermengen (Beispiel in VL mit der Bundesligatabelle) gebildet.
-- Durch die Join condition werden zusätzlich die Varianten rausgefiltert, die erst nach der Anschaffung des Exemplars gemahnt wurden. 