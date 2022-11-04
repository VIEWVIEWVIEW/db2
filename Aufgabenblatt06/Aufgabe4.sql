SELECT L.LeserOID, COALESCE(SUM(M.Betrag),'kein Betrag offen') as Mahnungsbetrag
FROM Leser as L
    left outer join Mahnung as M
        on L.LeserOID = M.LeserOID
GROUP BY L.LeserOID, L.Nachname