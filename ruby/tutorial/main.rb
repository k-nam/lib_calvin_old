require_relative 'container.rb'
require_relative 'iteration.rb'
require_relative 'regex.rb'
require_relative 'block.rb'

=begin
    Multiline comment
=end
#
def run_test(category, number)
    eval(category.capitalize + "Test.send('test_" + number + "')")
end

#run_test('container', '01')
#run_test('container', '02')
#run_test('container', '03')
#run_test('container', '04')

#run_test('iteration', '03')

run_test('regex', '01')
#run_test('regex', '02')

#run_test('block', '01')



