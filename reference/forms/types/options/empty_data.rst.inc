empty_data
~~~~~~~~~~

**tip**: ``mixed`` **varsayılan**: ``array()`` Eğer ``multiple`` ya da ``expanded``,  aksi durumda ``''`` 

Bu seçenek ``empty_value`` seçeneği seçildiğinde hangi değeri döndüreceğini
belirler.

Örneğin eğer ``gender`` alanını hiç bir seçenek seçilmediğinde ``null`` olacaksa
bunu şu şekilde yapabilirsiniz:

.. code-block:: php

    $builder->add('gender', 'choice', array(
        'choices' => array(
            'e' => 'Erkek',
            'k' => 'Kadın'
        ),
        'required'    => false,
        'empty_value' => 'Cinsiyeti Seçin',
        'empty_data'  => null
    ));
