require_relative 'container.rb'
require_relative 'regex.rb'
require_relative 'block.rb'


def run_test(category, number)
    eval(category.capitalize + "Test.send('test_" + number + "')")
end


#un_test('container', '01')

#run_test('regex', '01')
#run_test('regex', '02')

run_test('block', '01')

puts 'a'*3

