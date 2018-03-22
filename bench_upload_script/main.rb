require_relative 'test.rb'

server_url = "http://13.125.53.10:3000/runs/api"

#test_upload(server_url)

data_file_path = '../../bench_result/*'
filenames = Dir[data_file_path]
filenames.each do |filename|
  json_text = File.read(filename)
  http_post_json(server_url, json_text)
  File.delete(filename)
  puts 'Finished uploading: ' + filename
end
#puts filenames
#http_post_json(json_text, server_url)
