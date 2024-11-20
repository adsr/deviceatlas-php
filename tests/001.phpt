--TEST--
deviceatlas test
--INI--
deviceatlas.json_path={PWD}/data.json
deviceatlas.cache_size=1024
--EXTENSIONS--
deviceatlas
--FILE--
<?php
$props = deviceatlas_get_properties([
  'accept' => 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
  'accept-language' => 'en-US,en;q=0.9',
  'cache-control' => 'max-age=0',
  'downlink' => '10',
  'dpr' => '1',
  'ect' => '4g',
  'priority' => 'u=0, i',
  'rtt' => '0',
  'sec-ch-dpr' => '1',
  'sec-ch-ua' => '"Chromium";v="130", "Google Chrome";v="130", "Not?A_Brand";v="99"',
  'sec-ch-ua-arch' => '"x86"',
  'sec-ch-ua-bitness' => '"64"',
  'sec-ch-ua-full-version-list' => '"Chromium";v="130.0.6723.91", "Google Chrome";v="130.0.6723.91", "Not?A_Brand";v="99.0.0.0"',
  'sec-ch-ua-mobile' => '?0',
  'sec-ch-ua-platform' => '"Linux"',
  'sec-ch-ua-platform-version' => '"6.11.7"',
  'sec-fetch-dest' => 'document',
  'sec-fetch-mode' => 'navigate',
  'sec-fetch-site' => 'none',
  'sec-fetch-user' => '?1',
  'upgrade-insecure-requests' => '1',
  'user-agent' => 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36',
]);
var_dump($props);
?>
--EXPECT--
array(20) {
  ["isSetTopBox"]=>
  bool(false)
  ["isMobilePhone"]=>
  bool(false)
  ["isGamesConsole"]=>
  bool(false)
  ["isTablet"]=>
  bool(false)
  ["primaryHardwareType"]=>
  string(7) "Desktop"
  ["touchScreen"]=>
  bool(false)
  ["isTV"]=>
  bool(false)
  ["isEReader"]=>
  bool(false)
  ["isRobot"]=>
  bool(false)
  ["isBrowser"]=>
  bool(true)
  ["mobileDevice"]=>
  bool(false)
  ["isMediaPlayer"]=>
  bool(false)
  ["osAndroid"]=>
  bool(false)
  ["osiOs"]=>
  bool(false)
  ["osName"]=>
  string(5) "Linux"
  ["browserRenderingEngine"]=>
  string(5) "Blink"
  ["browserName"]=>
  string(6) "Chrome"
  ["devicePixelRatio"]=>
  string(1) "1"
  ["osVersion"]=>
  string(6) "6.11.7"
  ["browserVersion"]=>
  string(13) "130.0.6723.91"
}
