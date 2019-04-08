
wkeOnLoadUrlBegin(function(url, job){
	console.log("wkeOnLoadUrlBegin:" + url);
	
	var newUrl = url.replace("https://www.baidu.com", "https://www.hao123.com");
	if (newUrl != url) {
		wkeNetChangeRequestUrl(job, newUrl);
	}
	
	return false;
});


if (GlobalData("home page") == undefined) {
	wkeLoadUrl("https://www.baidu.com");
	GlobalData("home page", true);
}