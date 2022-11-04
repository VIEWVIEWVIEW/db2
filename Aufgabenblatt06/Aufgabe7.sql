SELECT DISTINCT L.Nachname as Nachname, V.vormerkdatum
FROM Leser as L
    left outer join Vormerkung as V
        on L.LeserOID = V.LeserOID
