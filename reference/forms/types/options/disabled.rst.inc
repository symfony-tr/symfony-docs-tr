disabled
~~~~~~~~
**tip**: ``boolean``  **varsayılan**: ``false``

Kullanıcının bir form alanını değiştirmesini istemiyorsanız disabled
seçeneğinin değerini true yapın. Bu alandaki gönderilen her veri 
dikkate alınmayacaktır.

.. code-block:: php

    use Symfony\Component\Form\TextField
      
    $field = new TextField('status', array(
        'data' => 'Eski Veri',
        'disabled' => true,
    ));
    $field->submit('Yeni Veri');

    // "Eski Veri" yazar
    echo $field->getData();


