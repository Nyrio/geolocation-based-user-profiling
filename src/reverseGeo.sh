curl "https://api.opencagedata.com/geocode/v1/json?key=e049e546eae34bde83ee40eade04d37e&q="${1}"%2C"${2}"&pretty=1&no_annotations=1" | jq ".results[0].formatted"

