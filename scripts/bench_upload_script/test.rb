require_relative 'http_post_json.rb'

def test_upload(url)
  json_string =
    '{
        "comment": "block_qsort is my implementation of BlockQuickSort. pdqsort is the official one.",
        "data": {
            "results": {
                "[lib_calvin] block-qsort": [
                    41.1138921004549,
                    34.0158234127035,
                    5.57038761001857
                ],
                "[lib_calvin] mergesort": [
                    18.2755618455978,
                    16.2324937222048,
                    3.11648763541166
                ],
                "pdqsort": [
                    41.7105687782461,
                    33.7992880653159,
                    5.65118541751839
                ],
                "std::sort": [
                    15.5180833155962,
                    14.434120551985,
                    3.4736408185365
                ],
                "std::stable_sort": [
                    17.0732568891915,
                    13.6834589299259,
                    3.66070861325018
                ]
            },
            "test_cases": [
                "int",
                "16Byte (key:int)",
                "24Byte (key:string)"
            ],
            "unit": "M/s (higher is better)"
        },
        "date": "20180318",
        "env": "Intel 4790K with 16GB ram, Windows 10, Visual C++ v141",
        "tags": [
            "sorting"
        ],
        "title": "Sorting"
    }'

  #json_string = '{"run": { "title":"aa", "tags":[1,2], "data":{"a1":1, "a2":2}}}'

  http_post_json(url, json_string)
end
