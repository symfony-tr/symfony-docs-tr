data
~~~~

**tip**: mixed **varsayılan**: İlgili nesnenin varsayılan alanı (eğer var ise)

Form yarattığınızda her form alanı, form'un domain nesnesinin ilgili sınıf değişkenindeki
(property) değere göre başlatılır(eğer bir nesne form'a bağlanırsa). Eğer
formun başlatılma (initial) değerini değiştirmek ya da sadce bir bağımsız form
alanındaki veriyi değiştirmek için data seçeneğine değer atamanız gerekir::

    $builder->add('token', 'hidden', array(
        'data' => 'abcdef',
    ));


