empty_value
~~~~~~~~~~~

**tip**: ``string`` ya da  ``Boolean``

Bu seçenek özel bir "boş" seçeneğinin (örn: "Lütfen Seçiniz") seçim 
listesinde olup olmayacağını belirler. Bu seçenek sadece eğer ``expanded``
ve ``multiple`` seçeneklerinin ikiside false olduğunda çalışır.

* Metin olarak "Lütfen Seçiniz" değeri ile boş bir seçenek ekler::

    $builder->add('states', 'choice', array(
        'empty_value' => 'Lütfen seçiniz.',
    ));

* Herhangi bir "boş" seçeneğinin görünmemesini garanti altına alır::

    $builder->add('states', 'choice', array(
        'empty_value' => false,
    ));

Eğer ``empty_value`` seçeneğine değer atanmazsa boş bir seçenek (yazısız olarak)
otomatik olarak atanır. Ancak bunun olması için ``required`` seçeneği
değerinin false olması gerekir::

    // boş (metinsiz) seçenek eklenecek
    $builder->add('states', 'choice', array(
        'required' => false,
    ));
