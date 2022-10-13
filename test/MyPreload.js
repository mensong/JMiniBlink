
wkeShowDevtools('WebInspector/inspector.html');

LoadPlugins("Plugin.MFCTest.dll");

wkeOnLoadUrlBegin(function(url, job){	
	
	var newUrl = url.replace("https://www.hao123.com", "https://www.baidu.com");
	if (newUrl != url) {
		console.log("把" + url + "重定向到" + newUrl);
		wkeNetChangeRequestUrl(job, newUrl);
	}
	
	return false;
});



wkeOnDocumentReady(function(frameId){
	var url = wkeGetFrameUrl(frameId);
	console.log(url);
	/*
	if (url == "http://plm.uat.meicloud.com/index"){
		alert(wkeGetCookie());
		alert(localStorage.getItem("userConfig"));
	}
	*/
	
	if (url == "https://www.baidu.com/"){
		var keyword = CallExtend("MFCTest", "foo1", 1,2,3,4);
		console.log(keyword);
		document.getElementById("kw").value = keyword;
	}
});


if (GlobalData("home page") == undefined) {
	wkeLoadUrl("https://www.baidu.com/");
	GlobalData("home page", true);
}
