label
~~~~~

**tip**: ``string`` **varsayılan**: etiket form alan tipi üzerinden *tahmin edilir*.

Ekrana basılacak olan alanın etiketini belirlemek için kullanılır. label 
aynı zamanda şablon içerisinden de direkt ayarlanabilir:
    
.. code-block:: jinja

    {{ form_label(form.name, 'İsminiz') }}
