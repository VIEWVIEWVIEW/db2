# Aufgabe 1

1. Dem Installer folgen und als network service user installieren. Der Server wird automatisch eingerichtet. Danach zu `localhost:27017` verbinden.

2.

```js
// create user collection
db.createCollection("users", {
  validator: {
    $jsonSchema: {
      required: ["last_name", "first_name", "addresses"],
      properties: {
        last_name: {
          bsonType: "string",
          description: "must be a string and is required",
        },
        first_name: {
          bsonType: "string",
          description: "must be a string and is required",
        },
        addresses: {
          bsonType: "array",
          minItems: 1,
          required: ["street", "city", "zip"],
          properties: {
            street: {
              bsonType: "string",
              description: "must be a string and is required",
            },
            city: {
              bsonType: "string",
              description: "must be a string and is required",
            },
            zip: {
              bsonType: "string",
              description: "must be a string and is required",
            },
          },
        },
      },
    },
  },
});

// create group
db.createCollection("groups", {
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["group_name"],
      properties: {
        group_name: {
          bsonType: "string",
          description: "must be a string and is required",
        },
        user_ids: {
          bsonType: "array",
          uniqueItems: true,
          items: {
            bsonType: "objectId",
            description: "must be an objectId and is required",
          },
        },
      },
    },
  },
});
```

Example user:

```js
/**
* Paste one or more documents here
*/
{
  "_id": {
    "$oid": "635fdb4c80611dbe8255ef83"
  },
  "last_name": "Richts",
  "first_name": "Marc",
  "addresses": [{
    "street": "Ernststraße 25",
    "city": "Iserlohn",
    "zip": "58644"
  }]
}
```

Example group:

```js
{
  "_id": {
    "$oid": "635fdb4c80611dbe8255ef83"
  },
  "group_name": "Students",
  "user_ids": [
    {
      "$oid": "635fdb4c80611dbe8255ef83"
    }
  ]
}
```

3. Application layer join:

```js
db.user
  .find({
    _id: {
      $in: db.gruppen.findOne({ name: "Students" }).user_ids,
    },
  })
  .toArray();
```

4. DB Dump siehe Verzeichnis

5.a
  - Database: Sammlung von Collections. Jede Collection hat einen Namen und kann beliebig viele Dokumente enthalten.
  - Collection: Sammlung von Dokumenten. Jedes Dokument hat eine eindeutige `_id` und kann beliebig viele Felder enthalten.

5.b JSON vs BSON
  - JSON ist ein Textformat, BSON ist ein Binärformat. BSON ist kompakter und schneller als JSON. Analog zu binary JSON in PostgreSQL.
  - BSON hat Datentypen für Date, Ints, Bools, Doubles etc. JSON hat nur Strings.

5.c Capped Collections
  - Capped Collections sind Ringbuffer mit einer feste Größe. Wenn die Collection voll ist, werden die ältesten Dokumente gelöscht. Anwendungsfall ist z.B. Logging, Rate Limiting, etc.
  - Ähnlich wie bei Redis gibt es auch klassische eviction Algorithmen wie LRU, LFU, etc.

5.d
  - Zuerst kontaktiert `mongos` alle bekannten `mongod` Instanzen und *queried* nach den Daten die zu dem Filter ``{"status": "A"}`` in der Collection ``orders`` passen.
  - Anschließend werden die Daten ``cust_id``s *gemappt*, ergo alle Daten, die die gleiche ``cust_id`` haben, werden zusammengefasst.
  - Am Ende werden die Daten *reduced*, also die Daten werden zusammengefasst. In diesem Fall wird die Summe der ``amount``s berechnet.

