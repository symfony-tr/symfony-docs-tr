.. index::
   single: Process

Process Bileşeni
=====================

    Process komponenti komutları alt-işlemler içerisinde çalıştırır.

Kurulum
-------

Bu bileşeni farklı yollar ile yükleyebilirsiniz:

* Resmi Git reposunu kullanarak (https://github.com/symfony/Process);
* PEAR ile yükleme ( `pear.symfony.com/Process`);
* Composer ile yükleme (Packagist 'deki `symfony/process`).

Kullanım
--------

:class:`Symfony\\Component\\Process\\Process` sınıfı komutları alt-işlem içerisinde çalıştırmaya izin verir::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->setTimeout(3600);
    $process->run();
    if (!$process->isSuccessful()) {
        throw new RuntimeException($process->getErrorOutput());
    }

    print $process->getOutput();

:method:`Symfony\\Component\\Process\\Process::run` metodu komutun farklı
platformlarda calıştırılması sırasında ki küçük farklılıkları ortadan kaldırır.

Uzun süreli bir komut çalıştırılırken (dosyaların rsync ile uzak sunucuya
gönderilmesi gibi), :method:`Symfony\\Component\\Process\\Process::run` metoduna
anonim bir fonksiyon geçirerek durumu son kullanıcıya gerçek zamanlı olarak iletebilirsiniz::

    use Symfony\Component\Process\Process;

    $process = new Process('ls -lsa');
    $process->run(function ($type, $buffer) {
        if ('err' === $type) {
            echo 'ERR > '.$buffer;
        } else {
            echo 'OUT > '.$buffer;
        }
    });

İzole bir şekilde PHP kodu çalıştırmak isterseniz, ``PhpProcess`` sınıfını
kullanabilirsiniz::

    use Symfony\Component\Process\PhpProcess;

    $process = new PhpProcess(<<<EOF
        <?php echo 'Hello World'; ?>
    EOF);
    $process->run();
