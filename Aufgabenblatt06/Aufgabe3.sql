select b.titel as titel, m.datum as mahnungsdatum
from buch as b
left outer join mahnung as m
    on b.buchoid = m.buchoid
group by b.titel
