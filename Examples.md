LSON Examples
====================================================================================================

The following are examples of the [LSON](./README.md) format.

  - Typed Elements: see examples [3], [5], [6]
  - Tables: see examples [2], [4]
  - Graphs: see examples [5], [6]

<br>

Example 1
----------
(from https://json.org/example.html)

### JSON
    {
        "glossary": {
            "title": "example glossary",
            "GlossDiv": {
                "title": "S",
                "GlossList": {
                    "GlossEntry": {
                        "ID": "SGML",
                        "SortAs": "SGML",
                        "GlossTerm": "Standard Generalized Markup Language",
                        "Acronym": "SGML",
                        "Abbrev": "ISO 8879:1986",
                        "GlossDef": {
                            "para": "A meta-markup language, used to create markup languages such as DocBook.",
                            "GlossSeeAlso": ["GML", "XML"]
                        },
                        "GlossSee": "markup"
                    }
                }
            }
        }
    }

### LSON
    {
        glossary: {
            title: "example glossary"
            GlossDiv: {
                title: S
                GlossList: {
                    GlossEntry: {
                        ID: SGML
                        SortAs: SGML
                        GlossTerm: "Standard Generalized Markup Language"
                        Acronym: SGML
                        Abbrev: ISO\ 8879:1986
                        GlossDef: {
                            para: "A meta-markup language, used to create markup languages such as DocBook."
                            GlossSeeAlso: [GML XML]
                        },
                        GlossSee: markup
                    }
                }
            }
        }
    }

<br>

Example 2
----------
(from https://json.org/example.html)

### JSON
    {
        "menu": {
            "id": "file",
            "value": "File",
            "popup": {
                "menuitem": [
                    {"value": "New", "onclick": "CreateNewDoc()"},
                    {"value": "Open", "onclick": "OpenDoc()"},
                    {"value": "Close", "onclick": "CloseDoc()"}
                ]
            }
        }
    }

### LSON
    {
        menu: {
            id: file
            value: File
            popup: {
                menuitem: [#
                    value   onclick :
                    New     CreateNewDoc()
                    Open    OpenDoc()
                    Close   CloseDoc()
                #]
            }
        }
    }

<br>

Example 3
----------
(from https://json.org/example.html)

### JSON
    {
        "widget": {
            "debug": "on",
            "window": {
                "title": "Sample Konfabulator Widget",
                "name": "main_window",
                "width": 500,
                "height": 500
            },
            "image": {
                "src": "Images/Sun.png",
                "name": "sun1",
                "hOffset": 250,
                "vOffset": 250,
                "alignment": "center"
            },
            "text": {
                "data": "Click Here",
                "size": 36,
                "style": "bold",
                "name": "text1",
                "hOffset": 250,
                "vOffset": 100,
                "alignment": "center",
                "onMouseUp": "sun1.opacity = (sun1.opacity / 100) * 90;"
            }
        }
    }

### LSON
    {
        widget: {
            debug: on
            window: {
                title: "Sample Konfabulator Widget"
                name: main_window
                width:  (count:500),
                height: (count:500)
            },
            image: {
                src: Images/Sun.png
                name: sun1
                hOffset: (int:250)
                vOffset: (int:250)
                alignment: (alignment:center)
            },
            text: {
                data: Click Here,
                size: (count:36),
                style: (fontWeight:bold),
                name: text1,
                hOffset: (int:250),
                vOffset: (int:100),
                alignment: (alignment:center),
                onMouseUp: ((mouseUp javaScript:
                    sun1.opacity = (sun1.opacity / 100) * 90;
                mouseUp))
            }
        }
    }

<br>

Example 4
----------
(from https://json.org/example.html)

### JSON
    {
        "menu": {
            "header": "SVG Viewer",
            "items": [
                {"id": "Open"},
                {"id": "OpenNew", "label": "Open New"},
                null,
                {"id": "ZoomIn", "label": "Zoom In"},
                {"id": "ZoomOut", "label": "Zoom Out"},
                {"id": "OriginalView", "label": "Original View"},
                null,
                {"id": "Quality"},
                {"id": "Pause"},
                {"id": "Mute"},
                null,
                {"id": "Find", "label": "Find..."},
                {"id": "FindAgain", "label": "Find Again"},
                {"id": "Copy"},
                {"id": "CopyAgain", "label": "Copy Again"},
                {"id": "CopySVG", "label": "Copy SVG"},
                {"id": "ViewSVG", "label": "View SVG"},
                {"id": "ViewSource", "label": "View Source"},
                {"id": "SaveAs", "label": "Save As"},
                null,
                {"id": "Help"},
                {"id": "About", "label": "About Adobe CVG Viewer..."}
            ]
        }
    }

### LSON
    {
        menu: {
            header: «SVG Viewer»
            items: [# [id=(null) label=""]:
                [Open]
                [OpenNew "Open New"]
                []
                [ZoomIn "Zoom In"]
                [ZoomOut "Zoom Out"]
                [OriginalView "Original View"]
                []
                [Quality]
                [Pause]
                [Mute]
                []
                [Find "Find..."]
                [FindAgain "Find Again"]
                [Copy]
                [CopyAgain "Copy Again"]
                [CopySVG "Copy SVG"]
                [ViewSVG "View SVG"]
                [ViewSource "View Source"]
                [SaveAs "Save As"]
                []
                [Help]
                [About "About Adobe CVG Viewer..."]
            #]
        }
    }

<br>

Example 5
----------
A graph example from
[json-graph-specification](https://github.com/jsongraph/json-graph-specification/) —
`car_graphs.json`:

### JSON
    {
      "graphs": [
        {
          "type": "car",
          "label": "Car Manufacturer Relationships",
          "nodes": [
            { "id": "nissan",   "label": "Nissan" },
            { "id": "infiniti", "label": "Infiniti" },
            { "id": "toyota",   "label": "Toyota" },
            { "id": "lexus",    "label": "Lexus" }
          ],
          "edges": [
            {
              "source": "nissan",
              "target": "infiniti",
              "relation": "has_luxury_division"
            },
            {
              "source": "toyota",
              "target": "lexus",
              "relation": "has_luxury_division"
            }
          ]
        },
        {
          "type": "car",
          "label": "Car Manufacturer Countries",
          "nodes": [
            { "id": "japan",  "label": "Japan" },
            { "id": "nissan", "label": "Nissan" },
            { "id": "toyota", "label": "Toyota" }
          ],
          "edges": [
            {
              "source": "nissan",
              "target": "japan",
              "relation": "country_of_origin"
            },
            {
              "source": "toyota",
              "target": "japan",
              "relation": "country_of_origin"
            }
          ]
        }
      ]
    }

### LSON
    {
      "graphs": [
        {
          type: car
          label: 'Car Manufacturer Relationships'
          graph: [% {
              nissan:   'Nissan'
              infiniti: 'Infiniti'
              toyota:   'Toyota'
              lexus:    'Lexus'
            } {
              nissan → infiniti: (relation:has_luxury_division)
              toyota → lexus:    (relation:has_luxury_division)
            }
          %]
        },
        {
          type: car,
          label: 'Car Manufacturer Countries'
          graph: [% {
              japan:  "Japan"
              nissan: "Nissan"
              toyota: "Toyota"
            } {
              nissan > japan: (relation:country_of_origin)
              toyota > japan: (relation:country_of_origin)
            }
          %]
        }
      ]
    }

<br>

Exmaple 6:
-----------
A graph example from
[json-graph-specification](https://github.com/jsongraph/json-graph-specification/) –
`usual_suspects.json`:

### JSON
    {
      "graph": {
        "type": "movie characters",
        "label": "Usual Suspects",
        "nodes": [
          {
            "id": "Roger Kint",
            "label": "Roger Kint",
            "metadata": { "nickname": "Verbal", "actor": "Kevin Spacey" }
          },
          {
            "id": "Keyser Söze",
            "label": "Keyser Söze",
            "metadata": { "actor": "Kevin Spacey" }
          }
        ],
        "edges": [
          {
            "source": "Roger Kint",
            "target": "Keyser Söze",
            "relation": "is"
          }
        ],
        "metadata": {
          "release year": "1995"
        }
      }
    }

### LSON
    {
      graph: {
        type:  'movie characters'
        label: 'Usual Suspects'
        graph: [%
          [# id             label           metadata
          :  'Roger Kint'   "Roger Kint"    { nickname:"Verbal" actor:"Kevin Spacey" }
             'Keyser Söze'  "Keyser Söze"   { actor: "Kevin Spacey" }
          #]
          {
            'Roger Kint' > 'Keyser Söze': (relation:is)
          }
        %]
        metadata: {
          'release year': 1995
        }
      }
    }



[1]: #example-1
[2]: #example-2
[3]: #example-3
[4]: #example-4
[5]: #example-5
[6]: #example-6
