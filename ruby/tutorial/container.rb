module ContainerTest
    def ContainerTest.test_01()
        empty1 = []
        empty2 = Array.new
        words1 = ['Calvin', 'Tom']
        words2 = %w{ Calvin Tom} # returns array (automatic split)
        puts words1, words2
    end

    def ContainerTest.test_02()
        apple = 'apple'
        dictionary = { apple => 'リンゴ' }
        puts dictionary['apple']
        puts dictionary['pear'].inspect
    end

    def ContainerTest.test_03()
        apple = 'apple'
        # Colon means key is symbol
        dictionary = { apple: 'リンゴ' }
        puts dictionary['apple'].inspect
        puts dictionary[:apple]
    end

    def ContainerTest.test_04()
        data = Hash.new(0)      #default value
        data['height'] = 7
        puts data['width']
    end
end