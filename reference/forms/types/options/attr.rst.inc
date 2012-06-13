attr
~~~~

**tip**: array **varsayılan**: Boş array

Eğer HTML alanları için ekstra nitelikler kullanmak istiyorsasnız ``attr`` 
seçeneğini kullanın. Bu HTML niteliğininin adını dizenin anahtar(key) değerine
alan birleştirilmiş bir array (associative array) dır. Bu özellik bir HTML
bileşenini ekrana basan widget'a özel bir css stili uygulamak istediğinizde 
kullanuışlı olabilir::

    $builder->add('body', 'textarea', array(
        'attr' => array('class' => 'tinymce'),
    ));



