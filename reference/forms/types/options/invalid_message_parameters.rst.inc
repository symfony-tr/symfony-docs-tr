invalid_message_parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

**tip**: ``array`` **varsayılan**: ``array()``

``invalid_message`` seçeneği ayarlandığında bazı değişkenlerin metin
içerisinde olmasına ihtiyacınız olacaktır.Bunu şu şekilde yer tutucular
ile yapabilirsiniz.

    $builder->add('some_field', 'some_type', array(
        // ...
        'invalid_message'            => 'You entered an invalid value - it should include %num% letters',
        'invalid_message_parameters' => array('%num%' => 6),
    ));
