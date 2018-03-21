require 'net/http'
require 'uri'
require 'json'

def http_post_json(url, json_text)
  uri = URI.parse(url)
  header = {'Content-Type': 'application/json'}
  http = Net::HTTP.new(uri.host, uri.port)
  request = Net::HTTP::Post.new(uri.request_uri, header)
  request.body = json_text
  #puts request.body
  response = http.request(request)
  #puts response
end
