format
~~~~~~

**tip**: ``integer`` ya da ``string`` **varsayılan**: ``IntlDateFormatter::MEDIUM``

``IntlDateFormatter`` sınıfından aktarılan seçenek, kullanıcının uygun formatta
veri girmesinde kullanılır. Bu `widget`_ seçeneğinin ``single_text`` olarak 
ayarlandığında önemli bir rol oynar ve kullanıcının veriyi hangi formatta
gireceğini belirler. Varsayılan olarak format kullanıcının geçerli yerel
bilgisinden faydalanılarak oluşturulur. Yani *bu format şekli farklı kullanıcılarda
farklı olarak belirlenir*. Eğer isterseniz format' ı bir metin özelliğinde bir
değer girerek de değiştirebilirsinğiz.

Geçerli formatlar için `Date/Time Format Syntax`_ 'ına bakın. Örneğin
bir metin kutusu içerisinde kullanıcı tarih bilgisini ``dd-MM-yyyy`` şeklinde 
girmesi gerekiyorsa bu seçeneği şu şekilde ayarlamalısınız::

    $builder->add('date_created', 'date', array(
        'widget' => 'single_text',
        'format' => 'dd-MM-yyyy',
    ));

.. _`Date/Time Format Syntax`: http://userguide.icu-project.org/formatparse/datetime#TOC-Date-Time-Format-Syntax
