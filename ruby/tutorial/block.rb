module BlockTest
    def BlockTest.test_01()
        def BlockTest.work(inputs)
            for input in inputs do
                yield (input)
            end   
        end
        
        work(['Calvin', 'Tom']) { |input| puts 'Working with: ' + input }
    end

end