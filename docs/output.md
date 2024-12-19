# JSON Output Format

This file describes the format for the JSON output file.

---

The main JSON structure is split in two objects:
 - The execution metadata.
 - An array of benchmark results.

```JSON
{
    "meta" : {
        // described by the context class
    },
    "data" : [
        { // bench 1
            "name": "bench1",
            // bench1 results
        },  
        { // bench 2
            "name": "bench2",
            // bench2 results
        }
    ]
}
```