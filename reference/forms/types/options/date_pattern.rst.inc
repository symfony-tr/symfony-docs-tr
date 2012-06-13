pattern
~~~~~~~

**tip**: ``string``

Bu seçenek sadece `widget`_ ``choice`` tipinde ayarlandığı zaman kullanılır.
Varsayılan pattern değeri `format`_ seçeneğine bağlıdır ve format pattern'i
içerisindeki ``M``, ``d`` ve ``y`` karakterlerini eşleştirmeye çalışır. Eğer
hiç bir şey bulunamazsa varsayılan karakter şekli ``{{ year }}-{{ month }}-{{ day }}``
olacaktır.

Bu seçeneğin içerisindeki semboller:

  * ``{{ year }}``:  ``year`` widget 'ı ile yer değiştirir.
  * ``{{ month }}``:  ``month`` widget'ı ile yer değiştiri.
  * ``{{ day }}``:  ``day`` widget'ı ile yer değiştirir.
