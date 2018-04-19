require_relative 'test.rb'

server_url = "http://bench.minami.im/runs/api"

#test_upload(server_url)

data_file_path = '../../bench_result/*'
filenames = Dir[data_file_path]
filenames.each do |filename|
  json_text = File.read(filename)
  http_post_json(server_url, json_text)
  File.delete(filename)
  puts 'Finished uploading: ' + filename
end
puts 'All finished'

